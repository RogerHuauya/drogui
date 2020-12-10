#include "blender.h"

#define PPV 800

bool act_dir;

void initBlender(blender *b, int i2c_air, int i2c_o2, int dir_port, int dir_pin, int stp_port, int stp_pin){
    initFlow(&(b -> air), i2c_air);
    initFlow(&(b -> o2), i2c_o2);
    initStepper(&(b->motor), dir_port, dir_pin, stp_port, stp_pin);   
}

void readFIO2(blender *b){
    readFlow(&(b->air));
    readFlow(&(b->o2));
    b -> value =  (b->o2).val / ( (b->air).val + (b->o2).val + 0.00001);
    b -> value = (79.0 * (b -> value) / 100.0 ) + 21;
    (b->motor).pos = FIO22Pos(b->value);
}

void stepBlender(blender *b, bool dir){
    if(dir != act_dir) act_dir = dir, setStepperDir(&(b->motor), dir);
    stepStepper(&(b->motor));
}


double FIO22Pos(double percent){
    return percent* 5;
}

double getBlenderPos(blender *b){
    return (b->motor).pos;
}
