#ifndef PRESS_REG_H
#define PRESS_REG_H


#define epsPressReg 1

#include "utils.h"
#include "stepper.h"
#include "sensors.h"

typedef struct _press_reg{
    stepper motor;
    double value;
} press_reg;

void initPressReg(press_reg *pr, int dir_port, int dir_pin, int stp_port, int stp_pin);
void setPressRegPress(press_reg *pr, press *p);
void stepPressReg(press_reg * pr, bool dir);

void readPressReg(press_reg* pr);
double pressReg2Pos(double pp);

double getPressRegPos(press_reg* pr);

#endif