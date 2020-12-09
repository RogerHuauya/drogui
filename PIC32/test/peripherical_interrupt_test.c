//#define PERIPHERICAL_INTERRUPT_TEST
#ifdef PERIPHERICAL_INTERRUPT_TEST

#include "config.h"
#include <xc.h>
#include "interrupt.h"
#include "io.h"

#define LED     PRTB, 11
#define LED2    PRTA, 10
#define BUTTON PRTD, 12

int main(){
    initPBCLK();
    initInterrupts(0, 4);
    pinMode(LED, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(BUTTON, OUTPUT);

    while(1){
        digitalWrite(BUTTON, 1);
        digitalWrite(LED2, 1);
        delayMs(1000);
        digitalWrite(BUTTON, 0);
        digitalWrite(LED2, 0);
        delayMs(1000);
    }
    return 0;
}

void intInterrupt(0, 4){
    digitalToggle(LED);
    clearIntFlag(0);
}


#endif