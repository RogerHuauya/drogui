#ifndef CONTROL_H
#define CONTROL_H

#include "utils.h"



typedef struct _pid{
    double kp[5], kd[5], ki[5];
    double threshold[5];
    double erri, errd, e_ant;
    unsigned long long tant;
    double isat, osat, dt;
} pid;

double computeIndexedPid(pid* p, double error, unsigned long long t, double h);
double computePid(pid* p, double error, unsigned long long t);
void resetPid(pid* p, double ti);
void initPid(pid* p, double kp, double kd, double ki,double ti,double isat,double osat);
void initPidConstants();

#endif