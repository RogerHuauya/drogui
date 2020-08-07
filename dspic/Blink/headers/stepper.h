#ifndef STEPPER_H    
#define STEPPER_H
#include <stdbool.h>
#include "config.h"
#include <xc.h>
#include <libpic30.h>
#include "io.h"


typedef struct _stepper{
    int p_dir, p_stp, port;
} stepper;


void initStepper(stepper *s, int port, int p_dir, int p_stp);
void stepStepper(stepper *s);
void setStepperDir(stepper *, bool dir);


#endif