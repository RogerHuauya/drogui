//#define IO_TEST
#ifdef IO_TEST
#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"

#define LED PRTD, 8


int main(){
    initConfig();
    pinMode(LED, OUTPUT);    
    while(1){
        digitalWrite(LED, HIGH);
        __delay_ms(500);
        digitalWrite(LED, LOW);
        __delay_ms(500);
    }
    return 0;
}



#endif