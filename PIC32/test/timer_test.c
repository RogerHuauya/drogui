//#define TIMER_TEST
#ifdef TIMER_TEST

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "timer.h"
#include "io.h"
#include "serial.h"
#define LED PRTD, 9

timer my_timer;

int main(void){
    
    initPBCLK();
    initSerial(115200, 4);
    pinMode(LED, OUTPUT);
    initTimer(&my_timer, 2, 2, DIV256);
    setTimerFrecuency(&my_timer, 10);

    while (true){
        serialWriteString("hola\n");
        delayMs(1000);
    }
    return 0;
}

void timerInterrupt(2, 2){
    digitalToggle(LED);
    clearTimerFlag(&my_timer);
}

#endif