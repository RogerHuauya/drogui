#include <xc.h>
#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "serial.h"
#include "timer.h"

char s[50];
int x = 0;
timer* my_timer;



int main(void){
    initConfig();
    initSerial();
    initTimer(my_timer, 1, DIV1, 1);
    setTimerFrecuency(my_timer, 1);
    pinMode(PRTB, 4, OUTPUT);
    while (true){
        sprintf(s,"Hola %05d\n", x++);
        digitalWrite(PRTB, 4, HIGH);
        serialWriteString(s);
        digitalWrite(PRTB, 4, LOW);
        __delay_ms(100);
    }
    return 0;
}



void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void){

    serialWriteString("Waaaaaaat\n");
        
    clearTimerFlag(my_timer);
}
