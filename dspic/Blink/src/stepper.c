#include "stepper.h"


void initStepper(stepper *s, int port, int p_dir, int p_stp){
    s -> p_dir = p_dir;
    s -> p_stp = p_stp;
    s -> port = port;
    pinMode(port, p_dir, OUTPUT);
    pinMode(port, p_stp, OUTPUT);
}

void setStepperDir(stepper *s, bool dir){
    digitalWrite(s->port , s->p_dir, dir);
}


void stepStepper(stepper *s){
    digitalWrite(s -> port, s->p_stp, 0);
    __delay_us(2);
    digitalWrite(s -> port, s->p_stp, 1);
}
