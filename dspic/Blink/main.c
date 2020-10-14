#define MAIN
#ifdef MAIN
#include <xc.h>
#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "serial.h"
#include "timer.h"
#include "control.h"
#include "pwm.h"
#include "i2c.h"
#include "utils.h"

#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0
#define I2Cclock 100000
char buffer[80];

#define R_REG  0x10
#define P_REG  0x11
#define Y_REG  0x12

i2c slave;

pwm m1, m2, m3, m4;

pid z_control;
pid x_control;
pid y_control;

pid roll_control;
pid pitch_control;
pid yaw_control;

serial Serial1, Serial2;


timer readSensors;
timer millis;

double roll, pitch, yaw;
volatile unsigned long time = 0;

void initializeSystem(){
    
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    initSerial(&Serial2, SERIAL2, 115200);

    initPid(&z_control, 1, 0, 0, 0, 10 , 100);
    initPid(&x_control, 1, 0, 0, 0, 10 , 100);
    initPid(&y_control, 1, 0, 0, 0, 10 , 100);
    
    initPid(&roll_control, 2, 0, 0, 0, 1, 100);
    initPid(&pitch_control, 2, 0, 0, 0, 1, 100);
    initPid(&yaw_control, 2, 0, 0, 0, 1, 100);
    
    resetPid(&roll_control, 0);
    
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
    
    initTimer(&readSensors, 2, DIV256, 6);
    setTimerFrecuency(&readSensors, 50);

    initTimer(&millis, 3, DIV256, 7);
    setTimerFrecuency(&millis, 1000);
    
    
    initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    i2c2Reg[0x01] = 0;

    __delay_ms(500);
    
    serialWriteString(&Serial1, "init");
    
}

void timerInterrupt(2){
    if(serialAvailable(&Serial2)){
        roll = 1.0*(serialParseInt(&Serial2))/180*pi;
        pitch = 1.0*(serialParseInt(&Serial2))/180*pi;
        yaw = 1.0*(serialParseInt(&Serial2))/180*pi;
    }
    clearTimerFlag(&readSensors);
}

void timerInterrupt(3){
    time++;
    clearTimerFlag(&millis);
}

double angle_dif(double angle1, double angle2);

int vel = 0;
double H, R, P, Y;
double M1, M2, M3, M4;
int main(void){
    
    initializeSystem();
    int val = 0;
    
    while(1){
       /*
        roll +1 +4 -2 -3 pi
        
        pitch +4 +2 -1 -3 0

        yaw   +1 +2 -3 -4 pi

       */
      
        H = (double) i2c2Reg[0x05];
        R = computePid(&roll_control, angle_dif(pi, roll), time);
        P = computePid(&pitch_control, angle_dif(0, pitch), time);
        Y = computePid(&yaw_control, angle_dif(pi, yaw), time);
        
        i2c2Reg[R_REG] = (uint8_t) ((roll/pi)*120+120);
        i2c2Reg[P_REG] = (uint8_t) ((pitch/pi)*120+120);
        i2c2Reg[Y_REG] = (uint8_t) ((yaw/pi)*120+120);
        
        R = P = Y = 0;
        M1 = H + R - P + Y;
        M2 = H - R + P + Y;
        M3 = H - R - P - Y;
        M4 = H + R + P - Y;
        
        setPwmDutyTime(&m1, min(max(M1,0), 100));
        setPwmDutyTime(&m2, min(max(M2,0), 100));
        setPwmDutyTime(&m3, min(max(M3,0), 100));
        setPwmDutyTime(&m4, min(max(M4,0), 100));

        __delay_ms(100);

    }
    return 0;
}

double angle_dif(double angle1, double angle2){
    if(angle1 > angle2){
        if((angle1 - angle2) > (2*pi - angle1 + angle2)) return -2*pi + angle1 - angle2;
        else return angle1 - angle2;
    }
    else{
        if((angle2 - angle1) > (2*pi - angle2 + angle1)) return 2*pi - angle1 + angle2;
        else return angle1 - angle2;
    }
}

#endif