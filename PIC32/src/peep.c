#include "peep.h"

#define PPV 800

press *my_p;
bool act_dir = FWD;
void initPeep(peep* pp, int dir_port, int dir_pin, int stp_port, int stp_pin){
    initStepper(&(pp -> motor), dir_port, dir_pin, stp_port, stp_pin);
}

void setPeepPress(peep* pp, press* p){
    my_p = p;
}

void stepPeep(peep *pr, bool dir){
    if(dir != act_dir) act_dir = dir, setStepperDir(&(pr->motor), dir);
    stepStepper(&(pr->motor));
}

void readPeep(peep* pp){
    pp->value = my_p -> val;
    (pp->motor).pos = peep2Pos(pp->value);
}

double peep2Pos(double pp){
    return pp*5;
}

double getPeepPos(peep* p){
    return (p->motor).pos;
}