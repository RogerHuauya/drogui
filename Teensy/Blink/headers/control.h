#ifndef CONTROL_H
#define CONTROL_H

#include "utils.h"
#include "filter.h"
#include <Arduino.h>


enum TYPES_PID {NORMAL = 0, P2ID = 1, INDEXED = 2, PIDABS = 4, D_FILTER = 8};
typedef struct _pid{
    double kp[5], kd[5], ki[5];
    double threshold[5];
    double erri, errd, err_ant2, err_ant1;
    unsigned long long tant;
    double isat, osat, dt;
    int type;
    filter f;
} pid;

double computePid(pid* p, double error, unsigned long long t, double h);
void resetPid(pid* p, unsigned long long ti);

void initPid(pid* p, double kp, double kd, double ki,unsigned long long ti,double isat,double osat, int tipo);
void initPid(pid* p, double kp, double kd, double ki,unsigned long long ti,double isat,double osat, int tipo, int n, double*a, double*b);

void initPidConstants();

#endif
