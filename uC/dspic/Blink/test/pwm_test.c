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
serial Serial1;
long long x = 0;

int main(void){
char s[50];
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    __delay_ms(1000);
    
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
    
    
    initPwm();
    serialWriteString(&Serial1, "init");

    char buff[50];
    long long pwm = 0;
    while (true){
        if(serialAvailable(&Serial1)){
            //serialWriteChar(&Serial1, serialReadChar(&Serial1));
            int res = serialParseInt(&Serial1, &pwm);
            sprintf(buff, "%lld\n", pwm);
            serialWriteString(&Serial1, buff);
        }
        //serialWriteString(&Serial1, "x\n");

        /*
        setPwmDutyTime(&m1, min(max(100,0), 100));
        setPwmDutyTime(&m2, min(max(100,0), 100));
        setPwmDutyTime(&m3, min(max(100,0), 100));
        setPwmDutyTime(&m4, min(max(100,0), 100));
        __delay_ms(8000);
        
        setPwmDutyTime(&m1, min(max(0,0), 100));
        setPwmDutyTime(&m2, min(max(0,0), 100));
        setPwmDutyTime(&m3, min(max(0,0), 100));
        setPwmDutyTime(&m4, min(max(0,0), 100));
        __delay_ms(10000);
        
        setPwmDutyTime(&m1, 5);
        setPwmDutyTime(&m2, 5);
        setPwmDutyTime(&m3, 5);
        setPwmDutyTime(&m4, 5);
        */
       setPwmDutyTime(&m1, min(max(pwm,0), 100));
       setPwmDutyTime(&m2, min(max(pwm,0), 100));
       setPwmDutyTime(&m3, min(max(pwm,0), 100));
       setPwmDutyTime(&m4, min(max(pwm,0), 100));
        
    }
    return 0;
}


#endif