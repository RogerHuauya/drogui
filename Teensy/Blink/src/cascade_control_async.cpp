#define CASCADE_CONTROL_ASYNC
#ifdef CASCADE_CONTROL_ASYNC

#include "..\headers\main.h"
#include "..\headers\kalman.h"
#include "..\headers\control.h"
#include "..\headers\pwm.h"
#include "..\headers\i2c.h"
#include "..\headers\utils.h"
#include "..\headers\timer.h"
#include "..\headers\registerMap.h"
#include "..\headers\filter.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>
#include <Adafruit_BMP3XX.h>


#define N 25
struct imustruct{
    float acc[3] = {0, 0, 0};
    float gyro[3] = {0, 0, 0};
    float mag[3] = {0, 0, 0};
};
imustruct bno_struct;
i2c slave;

pwm m1, m2, m3, m4;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Adafruit_BMP3XX bmp;

extern pid z_control, x_control, y_control;

float alt_memo[N], alt_fast, alt_slow = 0, alt_diff, sum = 0,alt_offs;
int alt_pointer = 0;
float sealevel;

timer timer_accel, timer_gyro, timer_mag, timer_main, timer_attitude;

volatile double roll, pitch, yaw, ax, ay, az, gx, gy, gz;
double roll_ant,pitch_ant,yaw_ant;

double err_act_x = 0,err_ant_x = 0, der_err_x = 0;
double err_act_y = 0,err_ant_y = 0, der_err_y = 0;
double err_act_z = 0,err_ant_z = 0, der_err_z = 0;


//Lowpass freq = 10Hz
double coeffA_10Hz[] = {-6.1252,17.2079,-28.7647,31.1789,-22.3898,10.3890,-2.8462,0.3526};
double coeffB_10Hz[] = {0.0027,-0.0085,0.0164,-0.0210,0.0231,-0.0210,0.0164,-0.0085,0.0027};

//Lowpass freq = 2Hz
double coeffB_2Hz[] = {0.0001, -0.0012, 0.0064, -0.0209, 0.0462, -0.0731, 0.0850, -0.0731, 0.0462, -0.0209, 0.0064, -0.0012, 0.0001};
double coeffA_2Hz[] = {-11.8247, 64.1475, -211.1035, 469.3812, -742.8519, 858.0511, -728.8477, 451.8482, -199.3839, 59.4422, -10.7503, 0.8919};


filter filter_gx, filter_gy, filter_gz;
filter filter_R, filter_P, filter_Y;


float x, y, z;
volatile unsigned long long time = 0;
bool led_state;

double  H, H_comp,R,P,Y, H_ref, X_C, Y_C, R_MAX = pi/22.0 , P_MAX = pi/22.0;
double M1,M2,M3,M4;

double roll_off = 0 , pitch_off = 0, yaw_off = 0, x_off = 0, y_off = 0, z_off = 0;
double roll_ref, pitch_ref, yaw_ref, x_ref, y_ref, z_ref;

pid roll2w, pitch2w, yaw2w; 
pid wroll_control, wpitch_control, wyaw_control;


void saturateM(double H){
    double f_max = 1;
    double arr_M[] = {M1, M2, M3, M4};
    for(int i = 0; i < 4 ; i++){
        double delta = max(max(arr_M[i] + H - 10000, -arr_M[i]-H), 0);
        f_max = max(f_max, abs(arr_M[i] / (abs(arr_M[i]) - delta + 0.0000001)) );
    }

    M1 = sqrt(M1 / f_max + H);
    M2 = sqrt(M2 / f_max + H);
    M3 = sqrt(M3 / f_max + H);
    M4 = sqrt(M4 / f_max + H);
}

void rampValue(double *var, double desired, double step){

    (*var) += fabs(desired - (*var) ) >= step  ? copysign(step, desired - (*var)) : (desired - (*var));
}    

imu::Vector<3> aux;
void accelInterrupt(){
    aux = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno_struct.acc[0] = aux.x();
    bno_struct.acc[1] = aux.y();
    bno_struct.acc[2] = aux.z();
}

void gyroInterrupt(){
    aux = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno_struct.gyro[0] = aux.x();
    bno_struct.gyro[1] = aux.y();
    bno_struct.gyro[2] = aux.z();
}

void magInterrupt(){
    aux = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
    bno_struct.mag[0] = aux.x();
    bno_struct.mag[1] = aux.y();
    bno_struct.mag[2] = aux.z();
}

void attitudeInterrupt(){
    
}
void sensorsInterrupt(){
	
    
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;

    sensors_event_t orientationData , linearAccelData, gyroscopeData;
    
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    bno.getEvent(&gyroscopeData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    
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

    gx = computeFilter(&filter_gx, gyroscopeData.gyro.x);
    gy = computeFilter(&filter_gy, gyroscopeData.gyro.y);
    gz = computeFilter(&filter_gz, gyroscopeData.gyro.z);
/*
    gx = gyroscopeData.gyro.x;
    gy = gyroscopeData.gyro.y;
    gz = gyroscopeData.gyro.z;

    Serial.print(gx);
    Serial.print(" ");
    Serial.println(gy);
*/
    setReg(GYRO_X, -gx);
    setReg(GYRO_Y, -gy);
    setReg(GYRO_Z, gz);


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

    rampValue(&z_ref, getReg(Z_REF), getReg(Z_REF_SIZE));

    H_ref = computePid(&z_control, z_ref - z, time,0) + getReg(Z_MG);
    rampValue(&H, H_ref, 0.2);

    //H_comp = H / (cos(roll)*cos(pitch));
    H_comp = H;

    rampValue(&roll_ref, getReg(ROLL_REF) + roll_off, 0.0015);
    rampValue(&pitch_ref, getReg(PITCH_REF) + pitch_off, 0.0015);
    yaw_ref = getReg(YAW_REF) + yaw_off;

    
    /*Serial.print('\t');
    Serial.println(roll);*/


    double wroll = computePid(&roll2w, angle_dif(roll_ref, roll), time, 0);
    double wpitch = computePid(&pitch2w, angle_dif(pitch_ref, pitch),time, 0);
    double wyaw = computePid(&yaw2w, angle_dif(yaw_ref, yaw),time, 0);

    setReg(GYRO_X_REF,wroll);
    setReg(GYRO_Y_REF,wpitch);
    setReg(GYRO_Z_REF,wyaw);

    /*if( fabs(wroll + gx) < 5 ) wroll = -gx;
    if( fabs(wpitch + gy) < 5 ) wpitch = -gy;
    if( fabs(wyaw - gz) < 5 ) wyaw = gz;*/

    R = computePid(&wroll_control, wroll + gx, time, 0);
    P = computePid(&wpitch_control, wpitch + gy, time, 0);
    Y = computePid(&wyaw_control, wyaw - gz, time, 0);
    
    setReg(DER_GYRO_X, wroll_control.errd);
    setReg(DER_GYRO_Y, wpitch_control.errd);

    Serial.print(wroll_control.errd);
    Serial.print("\t");
    Serial.println(wpitch_control.errd);
    
    err_act_x = wroll + gx;
    err_act_y =  wpitch + gy;

    setReg(ROLL_U, R);
    setReg(PITCH_U, P);
    setReg(YAW_U, Y);
    setReg(Z_U, H_comp);

    M1 =  -R - P - Y;
    M2 = -R + P  + Y;
    M3 =   R + P - Y;
    M4 = R - P + Y;

    saturateM(H_comp*H_comp);

    setReg(MOTOR_1, M1);
    setReg(MOTOR_2, M2);
    setReg(MOTOR_3, M3);
    setReg(MOTOR_4, M4);
    
    if(getReg(Z_REF) == 0 || (fabs(angle_dif(roll_ref, roll))> pi/9) || (fabs(angle_dif(pitch_ref, pitch))> pi/9)){
        alt_offs = alt_slow;
        setReg(Z_REF, 0);
        H = 0; z_ref = 0;
        M1 = M2 = M3 = M4 = 0;
        int index = getReg(PID_INDEX), var = getReg(PID_VAR);
        if(index >= 0) {
            switch(var){
    
                case PID_ROLL:
                    if(index == 0)
                        roll2w.kp[0] = getReg(ROLL_KP),  roll2w.ki[0] = getReg(ROLL_KI),  roll2w.kd[0] = getReg(ROLL_KD);
                    else
                        wroll_control.kp[0] = getReg(ROLL_KP),  wroll_control.ki[0] = getReg(ROLL_KI),  wroll_control.kd[0] = getReg(ROLL_KD);

                break;
                
                case PID_PITCH:
                    if(index == 0)
                        pitch2w.kp[0] = getReg(PITCH_KP),  pitch2w.ki[0] = getReg(PITCH_KI),  pitch2w.kd[0] = getReg(PITCH_KD);
                    else
                        wpitch_control.kp[0] = getReg(PITCH_KP),  wpitch_control.ki[0] = getReg(PITCH_KI),  wpitch_control.kd[0] = getReg(PITCH_KD);

                break;

                case PID_YAW:
                    if(index == 0)
                        yaw2w.kp[0] = getReg(YAW_KP),  yaw2w.ki[0] = getReg(YAW_KI),  yaw2w.kd[0] = getReg(YAW_KD);
                    else
                        wyaw_control.kp[0] = getReg(YAW_KP),  wyaw_control.ki[0] = getReg(YAW_KI),  wyaw_control.kd[0] = getReg(YAW_KD);

                break;
                
                case PID_Z:
                    z_control.kp[0] = getReg(Z_KP);
                    z_control.ki[0] = getReg(Z_KI);
                    z_control.kd[0] = getReg(Z_KD);
                break;
            }
            roll2w.N_filt = pitch2w.N_filt = yaw2w.N_filt = wroll_control.N_filt = wpitch_control.N_filt = wyaw_control.N_filt = getReg(N_FILTER);
        }
        resetPid(&roll2w, time);
        resetPid(&pitch2w, time);
        resetPid(&yaw2w, time);
        resetPid(&wroll_control, time);
        resetPid(&wpitch_control, time);
        resetPid(&wyaw_control, time);
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

    initPid(&roll2w, 0, 0, 0, time, 50, 40, NORMAL);
    initPid(&pitch2w, 0, 0, 0, time, 50, 40, NORMAL);
    initPid(&yaw2w, 0, 0, 0, time, 50, 40, NORMAL);

    initPid(&wroll_control, 0, 0, 0, time, 50, 10000, (D_FILTER & P2ID & D_INT), 13 , coeffA_10Hz, coeffB_10Hz);
    initPid(&wpitch_control, 0, 0, 0, time, 50, 10000, (D_FILTER & P2ID & D_INT), 13 , coeffA_10Hz, coeffB_10Hz);
    initPid(&wyaw_control, 0, 0, 0, time, 50, 10000, (D_FILTER & P2ID & D_INT), 13 , coeffA_10Hz, coeffB_10Hz);


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
    alt_offs = alt_slow;

    initTimer(&timer_accel, &accelInterrupt, 1000);
    initTimer(&timer_gyro, &gyroInterrupt, 500);
    initTimer(&timer_mag, &magInterrupt, 30);
    initTimer(&timer_attitude, &magInterrupt, 30);

    initTimer(&timer_main, &mainInterrupt, 500);

    setKalmanTsImu(0.01);
    setKalmanTsGps(1);
    initMatGlobal();

    initFilter(&filter_gx, 9 , coeffA_10Hz, coeffB_10Hz);
    initFilter(&filter_gy, 9 , coeffA_10Hz, coeffB_10Hz);
    initFilter(&filter_gz, 9 , coeffA_10Hz, coeffB_10Hz);


    delay(500);
    
}



int _main(void){

    initializeSystem();
    
    yaw_off = yaw;
    setReg(PID_INDEX, -1);
    setReg(PID_VAR, -1);
    setReg(N_FILTER, 50);

    while(1){
        if(timerReady(&timer_accel)) executeTimer(&timer_accel);
        if(timerReady(&timer_main)) executeTimer(&timer_main);
    }
    return 0;
}
#endif
