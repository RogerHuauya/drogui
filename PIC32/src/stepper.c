#include "stepper.h"

bool up = false;
void initStepper(stepper *s, int port_dir, int p_dir, int port_stp, int p_stp){
    s -> p_dir = p_dir;
    s -> p_stp = p_stp;
    s -> port_dir = port_dir;
    s -> port_stp = port_stp; 
    s -> pos = 0;
    s -> dir = 0;
    pinMode(port_dir, p_dir, OUTPUT);
    pinMode(port_stp, p_stp, OUTPUT);
}

void setStepperDir(stepper *s, bool dir){
    s -> dir = dir;
    digitalWrite(s->port_dir , s->p_dir, dir);
}


void halfStepStepper(stepper *s){
    
    if(up){
        if((s -> dir) == BWD) s -> pos --;
        else  s -> pos ++;
    }
    
    digitalToggle(s -> port_stp, s->p_stp);
    up = !up;
}

void stepStepper(stepper *s){
    
    if((s -> dir) == BWD) s -> pos --;
    else  s -> pos ++;

    digitalWrite(s -> port_stp, s->p_stp, HIGH);
    delayUs(10);
    digitalWrite(s -> port_stp, s->p_stp, LOW);
    
}