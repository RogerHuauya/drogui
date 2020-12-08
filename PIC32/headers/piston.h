#ifndef PISTON_H    
#define PISTON_H
#include <xc.h>
#include <stdbool.h>
#include "timer.h"
#include "stepper.h"
#include "utils.h"
#include "interrupt.h"


#define MAXLEN 100

typedef struct _piston{

    double vel, pos, vel_d;
    stepper s;
    timer velo, accel;
    int p_dis;
    bool dir;

} piston;


void setPiston(piston* mp);
void initPistonStepper(int port_dir, int p_dir, int port_stp,  int p_stp);
void initPistonVelTimer(int pre);
void initPistonAccelTimer(int pre, double freq);

void setVelPiston(piston*, double vel);
void stopPiston(piston*);
void moveHome(piston*);

double getPos(piston *p);
double getVeld();


#endif