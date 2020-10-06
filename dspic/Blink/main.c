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
#include "piston.h"
#include "control.h"
#include "pwm.h"

pwm m1, m2, m3, m4;

pid z_control;
pid x_control;
pid y_control;

pid th_control;
pid phi_control;
pid tri_control;

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
}

int main(void){
    
    return 0;
}

#endif