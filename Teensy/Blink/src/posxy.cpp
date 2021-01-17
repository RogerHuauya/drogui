#define POSXY
#ifdef POSXY

#include "..\headers\main.h"
#include "..\headers\kalman.h"
#include "..\headers\control.h"
#include "..\headers\pwm.h"
#include "..\headers\i2c.h"
#include "..\headers\utils.h"
#include "..\headers\timer.h"
#include "..\headers\registerMap.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>
#include <Adafruit_BMP3XX.h>


#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define N 25

#define SEALEVELPRESSURE_HPA (1013.25)

char buffer[80];

i2c slave;

pwm m1, m2, m3, m4;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Adafruit_BMP3XX bmp;

extern pid z_control, x_control, y_control;

extern pid roll_control, pitch_control, yaw_control;


float alt_memo[N], alt_fast, alt_slow = 0, alt_diff, sum = 0,alt_offs;
int alt_pointer = 0;
float sealevel;

timer timer_sensors, timer_main;

volatile double roll, pitch, yaw, ax, ay, az;
float x, y, z;
volatile unsigned long long time = 0;
bool led_state;

bool caso = true;

long long entrada = 0;
int dig = 0;

double  H, H_comp,R,P,Y, H_ref, X_C, Y_C, R_MAX = pi/22.0 , P_MAX = pi/22.0;
double M1,M2,M3,M4;
uint8_t haux = 0;

double roll_off = 0 , pitch_off = 0, yaw_off = 0, x_off = 0, y_off = 0, z_off = 0;
double roll_ref, pitch_ref, yaw_ref, x_ref, y_ref, z_ref;
long long pm = 0;

void saturateM(double H){
    double f_max = 1;
    double arr_M[] = {M1, M2, M3, M4};
    for(int i = 0; i < 4 ; i++){
        double delta = max(max(arr_M[i] + H - 100, -arr_M[i]-H), 0);
        f_max = max(f_max, abs(arr_M[i] / (abs(arr_M[i]) - delta + 0.0000001)) );
    }

    M1 = M1 / f_max + H;
    M2 = M2 / f_max + H;
    M3 = M3 / f_max + H;
    M4 = M4 / f_max + H;
}



void sensorsInterrupt(){
	
    
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;

    sensors_event_t orientationData , linearAccelData;
    
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    
    yaw = (float)orientationData.orientation.x*pi/180 - pi;
    pitch = (float)orientationData.orientation.y*pi/180;
    roll = (float)orientationData.orientation.z*pi/180;

    setReg(ROLL_VAL,(float)(roll));
    setReg(PITCH_VAL,(float)(pitch));
    setReg(YAW_VAL,(float)(yaw));

    ax = linearAccelData.acceleration.x;
    ay = linearAccelData.acceleration.y;
    az = linearAccelData.acceleration.z;

    setReg(ACC_X,(float)(ax));
    setReg(ACC_Y,(float)(ay));
    setReg(ACC_Z,(float)(az));

    uint8_t sys, gyro, accel, mag = 0;
    bno.getCalibration(&sys, &gyro, &accel, &mag);

    setReg(CAL_SYS, (float) sys);
    setReg(CAL_GYR, (float) gyro);
    setReg(CAL_ACC, (float) accel);
    setReg(CAL_MAG, (float) mag);


    alt_pointer %= N;
    sum -= alt_memo[alt_pointer];
    alt_memo[alt_pointer] = bmp.readAltitude(sealevel);
    sum += alt_memo[alt_pointer++];
    alt_fast = sum / N;

    alt_slow = alt_slow * 0.99 + alt_fast * 0.01;

    alt_diff = alt_fast - alt_slow;
    alt_diff = max(min(alt_diff, 1), -1);

    if (abs(alt_diff) >  0.20) alt_slow += alt_diff / 6.0;
    
    /*Serial.println("Sensors");
    Serial.print(roll);
    Serial.print("\t");
    Serial.print(pitch);
    Serial.print("\t");
    Serial.print(yaw);
    Serial.print("\t");
    Serial.println(alt_slow - alt_offs);*/

    if(getReg(START) > 0){
        kalmanUpdateIMU(ax, ay, az, roll, pitch, yaw);
        if(getReg(GPS_AVAILABLE) == 1) setReg(GPS_AVAILABLE, 0), kalmanUpdateGPS(getReg(GPS_X), getReg(GPS_Y), 0);    
    }
    else{
        clearKalman();
    }

    getPosition(&x, &y, &z);
    
    z = alt_slow - alt_offs;

    setReg(X_VAL, x);
    setReg(Y_VAL, y);
    setReg(Z_VAL, z);  

}

void mainInterrupt(){
    
    time += timer_main.period/1000;

    X_C = computePid(&x_control, -x, time, H);
    Y_C = computePid(&y_control, -y, time, H);

    roll_ref = Y_C*cos(yaw) + X_C*sin(yaw);
    pitch_ref = Y_C*sin(yaw) - X_C*cos(yaw);

    z_ref += fabs(getReg(Z_REF) - z_ref) >= getReg(Z_REF_SIZE)  ? copysign(getReg(Z_REF_SIZE), getReg(Z_REF) - z_ref) : 0;
    
    H_ref = computePid(&z_control, z_ref - z, time,0) + getReg(Z_MG);

    H += fabs(H_ref - H) >= 0.1  ? copysign(0.1, H_ref - H) : 0;

    H_comp = H / (cos(roll)*cos(pitch));
    
    double rel = roll_ref/(pitch_ref + 0.0000001);
    
    if( fabs(rel) < 1  ){
        
        if( fabs(pitch_ref) >= P_MAX  ){
        pitch_ref = copysign(P_MAX, pitch_ref);
        roll_ref = pitch_ref * rel;
        }
        
    }
    else{ 

        if( fabs(roll_ref) >= R_MAX  ){
        roll_ref = copysign(R_MAX, roll_ref);
        pitch_ref = roll_ref/rel;
        }
        
    }

    roll_ref = getReg(ROLL_REF) + roll_off;
    pitch_ref = getReg(PITCH_REF) + pitch_off;
    yaw_ref = getReg(YAW_REF) + yaw_off;
    

    R = computePid(&roll_control, angle_dif(roll_ref, roll), time, H_comp);
    P = computePid(&pitch_control, angle_dif(pitch_ref, pitch),time, H_comp);
    Y = computePid(&yaw_control, angle_dif(yaw_ref, yaw),time, H_comp);


    setReg(ROLL_U, R);
    setReg(PITCH_U, P);
    setReg(YAW_U, Y);
    setReg(Z_U, H_comp);

    M1 = R + P - Y;
    M2 = R - P + Y;
    M3 = - R - P - Y;
    M4 = - R + P + Y;
    
    saturateM(H_comp);
    
    if(getReg(Z_REF) == 0 || (fabs(angle_dif(roll_ref, roll))> pi/9) || (fabs(angle_dif(pitch_ref, pitch))> pi/9)){
        
        setReg(Z_REF, 0);
        H = 0; z_ref = 0;
        M1 = M2 = M3 = M4 = 0;
        int index = getReg(PID_INDEX), var = getReg(PID_VAR);
        if(index >= 0) {
            switch(var){
    
                case PID_ROLL:
                    roll_control.kp[index] = getReg(ROLL_KP);
                    roll_control.ki[index] = getReg(ROLL_KI);
                    roll_control.kd[index] = getReg(ROLL_KD);
                break;
                
                case PID_PITCH:
                    pitch_control.kp[index] = getReg(PITCH_KP);
                    pitch_control.ki[index] = getReg(PITCH_KI);
                    pitch_control.kd[index] = getReg(PITCH_KD);
                break;

                case PID_YAW:
                    yaw_control.kp[index] = getReg(YAW_KP);
                    yaw_control.ki[index] = getReg(YAW_KI);
                    yaw_control.kd[index] = getReg(YAW_KD);
                break;
                
                case PID_Z:
                    z_control.kp[0] = getReg(Z_KP);
                    z_control.ki[0] = getReg(Z_KI);
                    z_control.kd[0] = getReg(Z_KD);
                break;
            }
        }
        resetPid(&roll_control, time);
        resetPid(&pitch_control, time);
        resetPid(&yaw_control, time);
        resetPid(&x_control, time);
        resetPid(&y_control, time);
        resetPid(&z_control, time);
    }
    
    setReg(ROLL_U, (float) R);
    setReg(PITCH_U, (float) P);
    setReg(YAW_U, (float) Y);

    setPwmDutyTime(&m1, min(max(M1,0), 100));
    setPwmDutyTime(&m2, min(max(M2,0), 100));
    setPwmDutyTime(&m3, min(max(M3,0), 100));
    setPwmDutyTime(&m4, min(max(M4,0), 100));
    
}

void initializeSystem(){
    
    initOneshot125(&m1, 5);
    initOneshot125(&m2, 4);
    initOneshot125(&m3, 3);
    initOneshot125(&m4, 2);

    initPidConstants();

    if(!bno.begin()){
        Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1){Serial.println("Bno");delay(10);}
    }
    
    delay(1000);

    bno.setExtCrystalUse(true);
    
    initI2C(SLAVE, I2C1, 0x60);
    clearI2Cregisters(I2C1);
     
    if (!bmp.begin_I2C()) {  
        Serial.println("Could not find a valid BMP3 sensor, check wiring!");
        while (1){ Serial.println("BMP"); delay(1000);};
    }


    bmp.setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_2X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);
    bmp.setOutputDataRate(BMP3_ODR_100_HZ);
    sealevel = bmp.readPressure() / 100.0;

    for ( int i = 0; i < N; i++ ) alt_memo[i] = bmp.readAltitude(sealevel), sum += alt_memo[i] / N, delay(5);

    alt_slow = sum / N;
    alt_offs = alt_slow ;

    initTimer(&timer_sensors, &sensorsInterrupt, 50);
    initTimer(&timer_main, &mainInterrupt, 200);

    setKalmanTsImu(0.01);
    setKalmanTsGps(1);
    initMatGlobal();

    delay(500);
    
}



int _main(void){

    initializeSystem();
    
    yaw_off = yaw;
    setReg(PID_INDEX, -1);
    setReg(PID_VAR, -1);

    while(1){
        if(timerReady(&timer_sensors)) executeTimer(&timer_sensors);
        if(timerReady(&timer_main)) executeTimer(&timer_main);
    }
    return 0;
}
#endif
