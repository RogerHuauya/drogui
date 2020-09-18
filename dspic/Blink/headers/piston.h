#ifndef PISTON_H    
#define PISTON_H
#include <stdbool.h>
#include <xc.h>
#include "timer.h"
#include "analog.h"
#include "io.h"
#include "stepper.h"

#define HOME 10
#define PPV 800
typedef struct _piston{

    double vel, pos;
    double vel_d;
    stepper s;
    timer velo, accel;
    int p_dis;

} piston;

extern piston my_piston;

void initPistonStepper(piston *p, int port, int p_dir, int p_stp);
void initPistonVelTimer(piston *p, int n, int pre, int post);
void initPistonAccelTimer(piston *p, int n, int pre, int post, double freq);

void setVelPiston(piston*, double vel);
void stopPiston(piston*);
void moveHome(piston*);


#endif