#define PWM_TEST
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
int x = 0;

int main(void){

    initConfig();
    initSerial();
    initAnalog();

    initPwmPin(&m1, 3, 1, 0);
    setPwmPrescaler(2);
    setPwmDutyLimits(&m1, 125, 250);
    setPwmFrecuency(&m1, 3500); 
       
    setPwmDutyTime(&m1, 0);
    initPwm();

    __delay_ms(1000);
    pinMode(LED, OUTPUT);
    
    while (true){
        uint16_t an = analogRead(5);
        setPwmDutyTime(&m1, (100.0*an/(1<<12)));

        sprintf(s, "%lf\n", (100.0*an/(1<<12)));
        //serialWriteString(s);
        digitalToggle(LED);
        __delay_ms(50);
    }
    return 0;
}


#endif