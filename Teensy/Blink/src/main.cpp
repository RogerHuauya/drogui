//#define MAIN
#ifdef MAIN

#include "..\..\headers\main.h"

char buffer[80];

i2c slave;

pwm m1, m2, m3, m4;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

extern pid z_control, x_control, y_control;

extern pid roll_control, pitch_control, yaw_control;

IntervalTimer readSensors;
IntervalTimer milli;

volatile double roll, pitch, yaw;
double x, y, z;
volatile unsigned long long time = 0;
bool led_state;

adafruit_bno055_offsets_t off_imu;
void timer1Interrupt(){
	  sensors_event_t event;
    bno.getEvent(&event);
    
    digitalWrite(13, led_state);
    led_state = !led_state;
    
    yaw = (float)event.orientation.x*pi/180 - pi;
    pitch = (float)event.orientation.y*pi/180;
    roll = (float)event.orientation.z*pi/180;
    
    setReg(ROLL_VAL,(float)(roll));
    setReg(PITCH_VAL,(float)(pitch));
    setReg(YAW_VAL,(float)(yaw));

    uint8_t sys, gyro, accel, mag = 0;
    bno.getCalibration(&sys, &gyro, &accel, &mag);
    
    setReg(CAL_SYS, (float) sys);
    setReg(CAL_GYR, (float) gyro);
    setReg(CAL_ACC, (float) accel);
    setReg(CAL_MAG, (float) mag);

    
    Serial.print(sys);
    Serial.print("\t");
    Serial.print(gyro);
    Serial.print("\t");
    Serial.print(accel);
    Serial.print("\t");
    Serial.println(mag);     
}

void timer2Interrupt(){
    time++;
}

void initializeSystem(){

    initOneshot125(&m1, 2);
    initOneshot125(&m2, 3);
    initOneshot125(&m3, 4);
    initOneshot125(&m4, 5);

    initPidConstants();
    if(!bno.begin()){

        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }
    
    delay(1000);

    bno.setExtCrystalUse(true);

    off_imu.accel_offset_x = -29;
    off_imu.accel_offset_y = -18;
    off_imu.accel_offset_z = 22;
    off_imu.gyro_offset_x = -2;
    off_imu.gyro_offset_y = 0;
    off_imu.gyro_offset_z = 0;
    off_imu.mag_offset_x = 27;
    off_imu.mag_offset_y = 70;
    off_imu.mag_offset_z = 305;
    off_imu.accel_radius = 1000;
    off_imu.mag_radius = 353;



    bno.setSensorOffsets(off_imu);

    readSensors.begin(timer1Interrupt, 10000);
    readSensors.priority(0);
    milli.begin(timer2Interrupt, 1000);
    milli.priority(100);


    initI2C(SLAVE, I2C1, 0x60);
    clearI2Cregisters(I2C1);

    delay(500);
    
}


bool caso = true;

long long entrada = 0;
int dig = 0;



double  H,R,P,Y, H_ref;
double M1,M2,M3,M4;
uint8_t haux = 0;
double roll_off = 0 , pitch_off = 0, yaw_off = 0, x_off = 0, y_off = 0, z_off = 0;
double roll_ref, pitch_ref, yaw_ref, x_ref, y_ref, z_ref;
long long pm = 0;


int main(void){

    initializeSystem();
    delay(1000);
    
    yaw_off = yaw;
    setReg(PID_INDEX, -1);
    setReg(PID_VAR, -1);

    while(1){
        
        roll_ref = getReg(ROLL_REF) + roll_off;
        pitch_ref = getReg(PITCH_REF) + pitch_off;
        yaw_ref = getReg(YAW_REF) + yaw_off;

        z_ref += fabs(getReg(Z_REF) - z_ref) >= getReg(Z_REF_SIZE)  ? copysign(getReg(Z_REF_SIZE), getReg(Z_REF) - z_ref) : 0;
        
        H_ref = computePid(&z_control, z_ref - z, time,0) + getReg(Z_MG);

        H += fabs(H_ref - H) >= 0.1  ? copysign(0.1, H_ref - H) : 0;

        R = computePid(&roll_control, angle_dif(roll_ref, roll), time, H);
        P = computePid(&pitch_control, angle_dif(pitch_ref, pitch),time, H);
        Y = computePid(&yaw_control, angle_dif(yaw_ref, yaw),time, H);
        
        setReg(ROLL_U, R);
        setReg(PITCH_U, P);
        setReg(YAW_U, Y);
        setReg(Z_U, H);

        
        M1 = H + R + P - Y;
        M2 = H + R - P + Y;
        M3 = H - R - P - Y;
        M4 = H - R + P + Y;
        
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
            resetPid(&z_control, time);
        }
        
        setReg(ROLL_U, (float) R);
        setReg(PITCH_U, (float) P);
        setReg(YAW_U, (float) Y);

        setPwmDutyTime(&m1, min(max(M1,0), 100));
        setPwmDutyTime(&m2, min(max(M2,0), 100));
        setPwmDutyTime(&m3, min(max(M3,0), 100));
        setPwmDutyTime(&m4, min(max(M4,0), 100));
        
        delay(max((int) getReg(TS_CONTROL), 5));
		    //Serial.println((int)time);
        


        
        /*
         *  Test Motor Oneshot125
         * /
        setPwmDutyTime(&m1, min(max(10,0), 100));
        delay(1000);
        setPwmDutyTime(&m2, min(max(10,0), 100));
        delay(1000);
        setPwmDutyTime(&m3, min(max(10,0), 100));
        delay(1000);
        setPwmDutyTime(&m4, min(max(10,0), 100));
        delay(1000);

        
        setPwmDutyTime(&m1, min(max(0,0), 100));
        delay(1000);
        setPwmDutyTime(&m2, min(max(0,0), 100));
        delay(1000);
        setPwmDutyTime(&m3, min(max(0,0), 100));
        delay(1000);
        setPwmDutyTime(&m4, min(max(0,0), 100));
        delay(1000);*/

        
    }
    return 0;
}
#endif
