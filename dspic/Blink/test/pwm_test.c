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

pwm m1, m2; 
char s[50];
int x = 0;

int main(void){

    initConfig();
    initSerial();
    initAnalog();

    initPwmPin(&m1, 1, 1);
    initPwmPin(&m2, 1, 0);

    setPwmPrescaler(0);
    
    setPwmFrecuency(&m1, 8000);    
    setPwmFrecuency(&m2, 4000);    
    setPwmDuty(&m1, 20);
    setPwmDuty(&m2, 50);
    initPwm();

    
    __delay_ms(1000);
    pinMode(PRTB, 4, OUTPUT);
    while (true){
        uint16_t an = analogRead(5);

        setPwmDuty(&m1, 100.0*an/(1<<12));
        digitalWrite(PRTB, 4, HIGH);
        __delay_ms(50);
        digitalWrite(PRTB, 4, LOW);
        __delay_ms(50);
    }
    return 0;
}


#endif