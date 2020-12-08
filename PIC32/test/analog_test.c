//#define ANALOG_TEST
#ifdef ANALOG_TEST

#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "analog.h"
#include "serial.h"

#define LED PRTC, 8


char s[50];
int x = 0;

int main(void){
    
    initConfig();
    initSerial(9600);
    initAnalog();

    delayMs(1000);

    pinMode(LED, OUTPUT);
    while (true){
        
        digitalWrite(LED, HIGH);
        
        uint16_t ar = analogRead(5);
        sprintf(s, "value: %d\n", ar);
        serialWriteString(s);
        delayMs(50);
        digitalWrite(LED, LOW);
        delayMs(50);
    }
    return 0;
}


#endif
