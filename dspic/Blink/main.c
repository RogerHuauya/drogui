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
#include "imu.h"
#include "MahonyAHRS.h"
#include "utils.h"

#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0
#define I2Cclock 100000
char buffer[80];
    
imu myIMU;

pwm m1, m2, m3, m4;

pid z_control;
pid x_control;
pid y_control;

pid th_control;
pid phi_control;
pid tri_control;


timer readSensors;
double roll, pitch, yaw;


void initializeSystem(){
    
    initPid(&z_control, 1, 0, 0, 0, 10 , 100);
    initPid(&x_control, 1, 0, 0, 0, 10 , 100);
    initPid(&y_control, 1, 0, 0, 0, 10 , 100);
    
    initPid(&th_control, 1, 0, 0, 0, 10 , 100);
    initPid(&phi_control, 1, 0, 0, 0, 10 , 100);
    initPid(&tri_control, 1, 0, 0, 0, 10 , 100);
    
    
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
    
    initMPU9250(&myIMU, I2C1, I2Cclock);
    initTimer(&readSensors, 2, DIV256, 7);
    setTimerFrecuency(&readSensors, 50);

}

void timerInterrupt(2){
    if (readByteIMU(&myIMU, MPU, INT_STATUS)& 0x01){
        __delay_ms(1);
        readAll(&myIMU);
        MahonyAHRSupdate(   myIMU.gx * pi/180.0f,myIMU.gy * pi/180.0f, myIMU.gz * pi/180.0f,  
                                myIMU.ax, myIMU.ay, myIMU.az, 
                                myIMU.mx, myIMU.my, myIMU.mz);
        roll = *(getMahonyEuler());
        pitch = *(getMahonyEuler()+1);
        yaw  = *(getMahonyEuler()+2);
    }
    clearTimerFlag(&readSensors);
}



i2c slave;
int vel = 0;

int main(void){
    initConfig();
    initSerial();
    initializeSystem();
    
    //initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    __delay_ms(1000);
    serialWriteString("init");
    
    while(1){
        
        __delay_ms(10000);
    }
    return 0;
}

#endif