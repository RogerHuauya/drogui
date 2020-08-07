#include "stepper.h"

double vel = 0;
timer *velo, *accel;


void initStepper(){
    initTimer(velo, 0, 0, 0);
    initTimer(accel, 0, 0, 0);
}

void setDir(bool dir){
    PIN_DIR = dir;
}


void step(){
    PIN_STP = 1;
    Nop(), Nop();
    PIN_STP = 0;
}


void stopStepper(){
    setVelStepper(0);
}


void setVelStepper(double x){

}

