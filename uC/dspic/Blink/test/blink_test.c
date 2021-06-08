//#define BLINK_TEST
#ifdef BLINK_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#define LED PRTD, 8 

char s[50];

int main(){
    initConfig();
    pinMode(LED, OUTPUT);
    while(1){

        digitalWrite(LED, 1);
        __delay_ms(100);
        digitalWrite(LED, 0);
        __delay_ms(100);
    }
    return 0;
}

#endif