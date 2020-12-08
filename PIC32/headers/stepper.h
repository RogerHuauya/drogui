#ifndef STEPPER_H    
#define STEPPER_H
#include <stdbool.h>
#include "config.h"
#include <xc.h>
#include "utils.h"
#include "io.h"

enum dirs {BWD, FWD};

typedef struct _stepper{
    int p_dir, p_stp, port_dir, port_stp;
    long long pos;
    bool dir;
} stepper;


void initStepper(stepper *s, int port_dir, int p_dir, int port_stp, int p_stp);
void halfStepStepper(stepper *s);
void stepStepper(stepper *s);
void setStepperDir(stepper *, bool dir);


#endif