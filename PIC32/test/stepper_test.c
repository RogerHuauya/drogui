// #define STEPPER_TEST
#ifdef STEPPER_TEST

#include <xc.h>
#include "config.h"
#include "utils.h"
#include "io.h"
#include "serial.h"
#include "piston.h"
#include "utils.h"
#include "timer.h"

#define LED PRTB, 11

char s[80];
long long pos_d = 0;
timer my_timer;
piston my_piston;

int main(void){
    
    initPBCLK();
    delayMs(100);
    setPiston(&my_piston);
    initPistonStepper(PRTC, 1, PRTC , 4);

    initPistonVelTimer(DIV256);
    initPistonAccelTimer(DIV256, 2000);
    
    pinMode(LED, OUTPUT);
    pos_d = 0;
    
    delayMs(1000);
    
    while (true){
        
        setVelPiston(&my_piston, 1000);
        digitalToggle(LED);
        delayMs(100);
    }

    return 0;
}

#endif