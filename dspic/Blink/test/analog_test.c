#define ANALOG_TEST
#ifdef ANALOG_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "analog.h"
#include "serial.h"


char s[50];
int x = 0;

int main(void){
    
    initConfig();
    initSerial();
    initAnalog();

    __delay_ms(1000);

    pinMode(PRTB, 4, OUTPUT);
    while (true){
        
        digitalWrite(PRTB, 4, HIGH);
        
        uint16_t ar = analogRead(5);
        sprintf(s, "value: %lf\n", 3.3*ar/(1<<12));
        serialWriteString(s);
        __delay_ms(50);
        digitalWrite(PRTB, 4, LOW);
        __delay_ms(50);
    }
    return 0;
}


#endif