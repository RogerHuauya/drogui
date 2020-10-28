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

char buffer[80];

i2c slave;

pwm m1, m2, m3, m4;

sensor acc, ori;

pid z_control;
pid x_control;
pid y_control;

pid roll_control;
pid pitch_control;
pid yaw_control;

serial Serial1;

timer readSensors;
timer millis;

double roll, pitch, yaw;
volatile unsigned long long time = 0;

void initializeSystem(){
    
    initConfig();
    
    setPwmPrescaler(0);

    initPwmPin(&m1, PWM3_H);
    setPwmDutyLimits(&m1, 125, 250);
    setPwmFrecuency(&m1, 3500);
    setPwmDutyTime(&m1, 0);
    
    initPwmPin(&m2, PWM4_H);
    setPwmDutyLimits(&m2, 125, 250);
    setPwmFrecuency(&m2, 3500);
    setPwmDutyTime(&m2, 0);

    initPwmPin(&m3, PWM5_H);
    setPwmDutyLimits(&m3, 125, 250);
    setPwmFrecuency(&m3, 3500);
    setPwmDutyTime(&m3, 0);
    
    initPwmPin(&m4, PWM6_H);
    setPwmDutyLimits(&m4, 125, 250);
    setPwmFrecuency(&m4, 3500);
    setPwmDutyTime(&m4, 0);
    
    initPwm();

    initSerial(&Serial1, SERIAL1, 115200);

    initPid(&z_control, 1, 0, 0, 0, 10 , 100);
    initPid(&x_control, 1, 0, 0, 0, 10 , 100);
    initPid(&y_control, 1, 0, 0, 0, 10 , 100);
    
    initPid(&roll_control, 0, 0, 0, 0, 1 , 100);
    initPid(&pitch_control, 0, 0, 0, 0, 1 , 100);
    initPid(&yaw_control, 0, 0, 0, 0, 1 , 100);
    

    initMM7150();
    initAccel(&acc, 100, 20);
    initOrient(&ori, 100,20);
    
    initTimer(&readSensors, 2, DIV256, 3);
    setTimerFrecuency(&readSensors, 100);


    initTimer(&millis, 3, DIV256, 3);
    setTimerFrecuency(&millis, 1000);
    
    
    initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    i2c2Reg[0x01] = 0;

    i2c2Reg[0x02] = 0;

    i2c2Reg[0x03] = 0;
    i2c2Reg[0x04] = 0;
    i2c2Reg[0x05] = 0;
    
    i2c2Reg[0x06] = 0;
    i2c2Reg[0x07] = 0;
    i2c2Reg[0x08] = 0;

    i2c2Reg[0x09] = 0;
    i2c2Reg[0x0A] = 0;
    i2c2Reg[0x0B] = 0;

    __delay_ms(500);
    
    serialWriteString(&Serial1, "init");
    
}

void armingSequence(){
    for(int i = -100 ; i <= 100 ; i++){
        setPwmDutyTime(&m1, min(max(100 - abs(i),0), 100));
        setPwmDutyTime(&m2, min(max(100 - abs(i),0), 100));
        setPwmDutyTime(&m3, min(max(100 - abs(i),0), 100));
        setPwmDutyTime(&m4, min(max(100 - abs(i),0), 100));
        sprintf(buffer, "%d\n", 50 - abs(i));
        serialWriteString(&Serial1, buffer);
        __delay_ms(100);
    }
}

void getEuler(double q0,double q1,double q2, double q3);

bool caso = true;

long long entrada = 0;
int dig = 0;
void timerInterrupt(2){
    readOrient(&ori);        
    getEuler(ori.dDataW, ori.dDataX, ori.dDataY, ori.dDataZ);
    
    i2c2Reg[ROLL_DEG] = roll*180.0/pi+180;
    i2c2Reg[PITCH_DEG] = pitch*180.0/pi+180;
    i2c2Reg[YAW_DEG] = yaw*180.0/pi+180;

    clearTimerFlag(&readSensors);
}

void timerInterrupt(3){
    time++;
    clearTimerFlag(&millis);
}

double angle_dif(double angle1, double angle2);

double  H,R,P,Y;
double M1,M2,M3,M4;
uint8_t haux = 0;
double roll_off, pitch_off, yaw_off;
long long pm = 0;


int main(void){
    
    initializeSystem();
    int val = 0;
    int Kp, Ki, Kd, roll_d, pitch_d, yaw_d;
    __delay_ms(1000);
    roll_off = roll;
    pitch_off = pitch;
    yaw_off = yaw;
    //armingSequence();
    while(1){

        H += fabs(i2c2Reg[0x02] - H) >= 0.1  ? copysign(0.1, i2c2Reg[0x02] - H) : 0;
        
        R = computePid(&roll_control, angle_dif(-pi, roll), time);
        P = computePid(&pitch_control, angle_dif(0, pitch), time);
        Y = computePid(&yaw_control, angle_dif(yaw_off, yaw), time);
        
        M1 = H + R - P - Y;
        M2 = H - R - P + Y;
        M3 = H - R + P - Y;
        M4 = H + R + P + Y;
        if(i2c2Reg[0x02] == 0){
            H = 0;
            M1 = M2 = M3 = M4 = 0;
            
            roll_control.kp = i2c2Reg[0x03];
            roll_control.ki = i2c2Reg[0x04];
            roll_control.kd = i2c2Reg[0x05];
            
            pitch_control.kp = i2c2Reg[0x06];
            pitch_control.ki = i2c2Reg[0x07];
            pitch_control.kd = i2c2Reg[0x08];
            
            yaw_control.kp = i2c2Reg[0x09];
            yaw_control.ki = i2c2Reg[0x0A];
            yaw_control.kd = i2c2Reg[0x0B];

            resetPid(&roll_control, time);
            resetPid(&pitch_control, time);
            resetPid(&yaw_control, time);
        }
        setPwmDutyTime(&m1, min(max(M1,0), 100));
        setPwmDutyTime(&m2, min(max(M2,0), 100));
        setPwmDutyTime(&m3, min(max(M3,0), 100));
        setPwmDutyTime(&m4, min(max(M4,0), 100));
        
        sprintf(buffer, "%.3lf %.3lf %.3lf %.3lf\n", H, R, P, Y);
        serialWriteString(&Serial1, buffer);
        __delay_ms(100);

    }
    return 0;
}
/*
    180, -170
    -170, 180
    180, 160
    160, 180
*/

double angle_dif(double angle1, double angle2){
    if(angle1 > angle2){
        if((angle1 - angle2) > (2*pi - angle1 + angle2)) return -2*pi + angle1 - angle2;
        else return angle1 - angle2;
    }
    else{
        if((angle2 - angle1) > (2*pi - angle2 + angle1)) return 2*pi - angle2 + angle1;
        else return angle1 - angle2;
    }
}

void getEuler(double q0, double q1, double q2, double q3){
	
    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q0 * q1 + q2 * q3);
    double cosr_cosp = 1 - 2 * (q1 * q1 + q2 * q2);
    roll = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (q0 * q2 - q3 * q1);
    if (abs(sinp) >= 1)
        pitch = copysign(pi / 2, sinp); // use 90 degrees if out of range
    else
        pitch = asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q0 * q3 + q1 * q2);
    double cosy_cosp = 1 - 2 * (q2 * q2 + q3 * q3);
    yaw = atan2(siny_cosp, cosy_cosp);
}
#endif