#ifndef BLENDER_H
#define BLENDER_H

#define epsBlender 1

#include <xc.h>
#include "sensors.h"
#include "stepper.h"

typedef struct _blender{    

    flow air, o2;
    stepper motor;
    double value;

} blender;

void initBlender(blender *b, int i2c_air, int i2c_o2, int dir_port, int dir_pin, int stp_port, int stp_pin);
void readFIO2(blender *b);
void stepBlender(blender *b, bool dir);
double FIO22Pos(double percent);
double getBlenderPos(blender *b);

#endif