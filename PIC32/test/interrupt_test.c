//#define INTERRUPT_TEST
#ifdef INTERRUPT_TEST

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "io.h"
#include "serial.h"
#include "piston.h"
#include "utils.h"

#define LED PRTB, 11
#define PIN PRTD, 0

int main(void){
    pinMode(PIN, INPUT);
    pinMode(LED, OUTPUT);    
    while (true){
        if(digitalRead(PIN)) digitalWrite(LED, HIGH);
        else digitalWrite(LED, LOW);
    }

    return 0;
}

#endif