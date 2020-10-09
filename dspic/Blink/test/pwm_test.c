//#define PWM_TEST
#ifdef PWM_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "pwm.h"
#include "serial.h"
#include "analog.h"

#define LED PRTD, 8
pwm m1, m2, m3, m4; 

long long x = 0;

int main(void){
char s[50];
    initConfig();
    initSerial();

    initPwmPin(&m1, PWM3_H);
    initPwmPin(&m2, PWM4_H);
    initPwmPin(&m3, PWM5_H);
    initPwmPin(&m4, PWM6_H);
    setPwmPrescaler(0);

    setPwmDutyLimits(&m1, 125, 250);
    setPwmDutyLimits(&m2, 125, 250);
    setPwmDutyLimits(&m3, 125, 250);
    setPwmDutyLimits(&m4, 125, 250);
    setPwmFrecuency(&m1, 3500);
    setPwmFrecuency(&m2, 3500);
    setPwmFrecuency(&m3, 3500);
    setPwmFrecuency(&m4, 3500);
    setPwmDutyTime(&m1, 0);
    setPwmDutyTime(&m2, 0);
    setPwmDutyTime(&m3, 0);
    setPwmDutyTime(&m4, 0);
    
    /*
    setPwmDutyLimits(&m2, 125, 250);
    setPwmFrecuency(&m2, 3500);
    setPwmDutyTime(&m2, 0);*/
    
    initPwm();

    __delay_ms(1000);
    pinMode(LED, OUTPUT);
    
    while (true){
        setPwmDutyTime(&m1, min(max(5,0), 100));
        setPwmDutyTime(&m2, min(max(5,0), 100));
        setPwmDutyTime(&m3, min(max(5,0), 100));
 
        setPwmDutyTime(&m4, min(max(5,0), 100));
        //setPwmDutyTime(&m4, min(max(5,0), 100));
        digitalToggle(LED);
        __delay_ms(50000);
    }
    return 0;
}


#endif