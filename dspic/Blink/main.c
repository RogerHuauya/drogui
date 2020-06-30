//#define MAIN
#ifdef MAIN
#include <xc.h>
#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include "io.h"
#include "serial.h"
#include "timer.h"
#include "piston.h"

char s[50];
int x = 0;
timer my_timer, t;
piston my_piston;

int main(void){
    
    initConfig();
    initSerial();
    initTimer(&my_timer,1, DIV256, 1);
    initPistonStepper(&my_piston, PRTB, 10, 11);
    initPistonVelTimer(&my_piston, 3, DIV256, 3);
    initPistonAccelTimer(&my_piston, 2, DIV256, 2, 2000);

    setTimerFrecuency(&my_timer, 4);

    __delay_ms(1000);

    double set_vel = 0;
    char c;
    pinMode(PRTB, 4, OUTPUT);
    while (true){
        
        
        if(serialAvailable()){
            while(serialAvailable()){
                set_vel *= 10;
                set_vel += (double) (serialReadChar() - '0');
            }
            setVelPiston(&my_piston, set_vel);
            //sprintf(s, "vel: %lf\n",set_vel);
            //serialWriteString(s);
            set_vel = 0;
        }

        digitalWrite(PRTB, 4, HIGH);
        __delay_ms(500);
        digitalWrite(PRTB, 4 , LOW);
        __delay_ms(500);

    }
    return 0;
}


void timerInterrupt(1){

    sprintf(s,"actual vel: %lf \n", my_piston.vel);
    serialWriteString(s);    
    clearTimerFlag(&my_timer);
}


void timerInterrupt(2){

    if(my_piston.vel != my_piston.vel_d){

        if(my_piston.vel_d > my_piston.vel) my_piston.vel ++;
        else if(my_piston.vel_d < my_piston.vel) my_piston.vel --;
        setTimerFrecuency(&my_piston.velo, my_piston.vel*PPV/60);
    
    }
    
    clearTimerFlag(&my_piston.accel);
}

void timerInterrupt(3){
	stepStepper(&my_piston.s);
	clearTimerFlag(&my_piston.velo);
}

#endif