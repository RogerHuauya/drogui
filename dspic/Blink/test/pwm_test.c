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
pwm m1, m2; 
char s[50];
long long x = 0;

int main(void){

    initConfig();
    initSerial();
    initAnalog();

    initPwmPin(&m1, 3, 1, 0);
    setPwmPrescaler(0);
    setPwmDutyLimits(&m1, 125, 250);
    setPwmFrecuency(&m1, 3500); 
       
    setPwmDutyTime(&m1, 0);
    initPwm();

    __delay_ms(1000);
    pinMode(LED, OUTPUT);
    
    while (true){
        if(serialAvailable()){
            
            x = serialParseInt();
            sprintf(s, "pwm: %lld\n", x);
            serialWriteString(s);
            setPwmDutyTime(&m1, min(max(x,0), 100));
        }
        digitalToggle(LED);
        __delay_ms(50);
    }
    return 0;
}


#endif