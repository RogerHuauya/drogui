#ifndef CONTROL_H
#define CONTROL_H

#include "utils.h"
#include <Arduino.h>


enum TYPES_PID {NORMAL = 0, P2ID = 1, INDEXED = 2, PIDABS = 4};
typedef struct _pid{
    double kp[5], kd[5], ki[5];
    double threshold[5];
    double erri, errd, err_ant2, err_ant1;
    unsigned long long tant;
    double isat, osat, dt;
    int type;
} pid;

double computePid(pid* p, double error, unsigned long long t, double h);
void resetPid(pid* p, double ti);
void initPid(pid* p, double kp, double kd, double ki,double ti,double isat,double osat, int tipo);
void initPidConstants();

#endif
