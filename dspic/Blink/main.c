#define MAIN
#ifdef MAIN

#include <xc.h>
#include "config.h"
#include <libpic30.h>

#include "io.h"
#include "serial.h"
#include "timer.h"
#include "control.h"
#include "MM7150.h"
#include "pwm.h"
#include "i2c.h"
#include "utils.h"
#include "registerMap.h"
#include "bmp280.h"
char buffer[80];

i2c slave;

pwm m1, m2, m3, m4;
sensor acc, gyro, ori;

extern pid z_control, x_control, y_control;

extern pid roll_control, pitch_control, yaw_control;

serial Serial1;

timer readSensors;
timer millis;
timer readPress;

double roll, pitch, yaw, z;
volatile unsigned long long time = 0;

void initializeSystem(){
    
    initConfig();
    
    setPwmPrescaler(0);

    initOneshot125(&m1, PWM3_H);
    initOneshot125(&m2, PWM4_H);
    initOneshot125(&m3, PWM5_H);
    initOneshot125(&m4, PWM6_H);
    
    initPwm();

    initSerial(&Serial1, SERIAL1, 115200);

    initPidConstants();

    initMM7150();
    initAccel(&acc, 100, 20);
    initOrient(&ori, 50, 10);

    initBmp280();

    initTimer(&readSensors, 2, DIV256, 4);
    setTimerFrecuency(&readSensors, 100);

    initTimer(&readPress, 4, DIV256, 3);
    setTimerFrecuency(&readPress, 20);

    initTimer(&millis, 3, DIV256, 3);
    setTimerFrecuency(&millis, 1000);
    
    
    initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    clearI2Cregisters(I2C2);

    __delay_ms(500);
    
    serialWriteString(&Serial1, "init");
    
}


bool caso = true;

long long entrada = 0;
int dig = 0;
void timerInterrupt(2){
    readOrient(&ori);        
    getEuler(ori.dDataW, ori.dDataX, ori.dDataY, ori.dDataZ, &roll, &pitch, &yaw);

    setReg(ROLL_VAL,(float)(roll));
    setReg(PITCH_VAL,(float)(pitch));
    setReg(YAW_VAL,(float)(yaw));

    clearTimerFlag(&readSensors);
}

int32_t raw_press, raw_temp;
float press, temper, press_ref = 0;
int med = 0;

void timerInterrupt(4){
    
    raw_temp = bmpReadTemperature();
    raw_press = bmpReadPressure();
    temper = bmp280CompensateTemperature(raw_temp);
    press = bmp280CompensatePressure(raw_press);
    setReg(RAW_TEMP, raw_temp);
    setReg(TEMP_ABS, temper);
    setReg(RAW_PRESS, raw_press);
    setReg(PRESS_ABS, press);

    if(med < 10){
        press_ref += press/10.0, z = 0;        
        med++;
    }
    else z = 44330 *(1-pow(1.0*press/press_ref, 0.1903));
    
    if(z < 0) press_ref = press;

    setReg(RAW_TEMP, med);
    setReg(Z_VAL, z);
    clearTimerFlag(&readPress);
}

void timerInterrupt(3){
    time++;
    clearTimerFlag(&millis);
}


double  H,R,P,Y, H_ref;
double M1,M2,M3,M4;
uint8_t haux = 0;
double roll_off = -3.09995788 , pitch_off = 0.0170128063, yaw_off = 0, x_off = 0, y_off = 0, z_off = 0;
double roll_ref, pitch_ref, yaw_ref, x_ref, y_ref, z_ref;
long long pm = 0;


int main(void){
    
    initializeSystem();
    __delay_ms(1000);
    
    yaw_off = yaw;
    setReg(PID_INDEX, -1);

    while(1){
        roll_ref = getReg(ROLL_REF) + roll_off;
        pitch_ref = getReg(PITCH_REF) + pitch_off;
        yaw_ref = getReg(YAW_REF) + yaw_off;

        z_ref += fabs(getReg(Z_REF) - z_ref) >= getReg(Z_REF_SIZE)  ? copysign(getReg(Z_REF_SIZE), getReg(Z_REF) - z_ref) : 0;
        
        H_ref = computePid(&z_control, z_ref - z, time) + getReg(Z_MG);

        H += fabs(H_ref - H) >= 0.1  ? copysign(0.1, H_ref - H) : 0;

        R = computeIndexedPid(&roll_control, angle_dif(roll_ref, roll), time, H);
        P = computeIndexedPid(&pitch_control, angle_dif(pitch_ref, pitch),time, H);
        Y = computeIndexedPid(&yaw_control, angle_dif(yaw_ref, yaw),time, H);
        
        setReg(ROLL_U, R);
        setReg(PITCH_U, P);
        setReg(YAW_U, Y);
        setReg(Z_U, H);

        
        M1 = H + R - P - Y;
        M2 = H - R - P + Y;
        M3 = H - R + P - Y;
        M4 = H + R + P + Y;
        
        if(getReg(Z_REF) == 0 || (fabs(angle_dif(roll_ref, roll))> pi/9) || (fabs(angle_dif(pitch_ref, pitch))> pi/9)){
            
            setReg(Z_REF, 0);
            H = 0; z_ref = 0;
            M1 = M2 = M3 = M4 = 0;
            int index = getReg(PID_INDEX), var = getReg(PID_VAR);
            if(index >= 0) {
                switch(PID_VAR){
                    case ROLL:
                        roll_control.kp[index] = getReg(ROLL_KP);
                        roll_control.ki[index] = getReg(ROLL_KI);
                        roll_control.kd[index] = getReg(ROLL_KD);
                    break;
                    
                    case PITCH:
                        pitch_control.kp[index] = getReg(PITCH_KP);
                        pitch_control.ki[index] = getReg(PITCH_KI);
                        pitch_control.kd[index] = getReg(PITCH_KD);
                    break;

                    case YAW:
                        yaw_control.kp[index] = getReg(YAW_KP);
                        yaw_control.ki[index] = getReg(YAW_KI);
                        yaw_control.kd[index] = getReg(YAW_KD);
                    break;
                    
                    case Z:
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
        
        //sprintf(buffer, "%.3lf %.3lf %.3lf %.3lf\n", H, R, P, Y);
        //sprintf(buffer, "%.3lf %.3lf %.3lf\n", roll, pitch, yaw);
        //serialWriteString(&Serial1, buffer);
        __delay_ms(max((int) getReg(TS_CONTROL), 5));

    }
    return 0;
}

#endif