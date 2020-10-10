#ifndef CONTROL_H
#define CONTROL_H

#include "utils.h"

typedef struct _pid{
    double kp, kd, ki;
    double erri, errd, e_ant;
    unsigned long tant;
    double isat, osat, dt;
} pid;

double computePid(pid* p, double error, unsigned long t);
void resetPid(pid* p, double ti);
void initPid(pid* p, double kp, double kd, double ki,double ti,double isat,double osat);

#endif