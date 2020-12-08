#include "press_reg.h"

#define PPV 800

press *my_press;
bool act_dir;

void initPressReg(press_reg *pr, int dir_port, int dir_pin, int stp_port, int stp_pin){
    initStepper(&(pr->motor), dir_port, dir_port, stp_port, stp_pin);
}

void setPressRegPress(press_reg *pr, press *p){
    my_press = p;
}

void stepPressReg(press_reg *pr, bool dir){
    if(dir != act_dir) act_dir = dir, setStepperDir(&(pr->motor), dir);
    stepStepper(&(pr->motor));
}


void readPressReg(press_reg* pr){
    pr -> value = my_press->val;
    (pr -> motor).pos = pressReg2Pos(pr -> value);
}

double pressReg2Pos(double preg){
    return preg*5;
}

double getPressRegPos(press_reg* pr){
    return (pr->motor).pos;
}