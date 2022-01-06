#ifndef CONTROL_H
#define CONTROL_H

#include <bits/stdc++.h>

#define EPS 1e-10

enum TYPES_PID {NORMAL = 0, P2ID = 1, INDEXED = 2, PIDABS = 4, D_FILTER = 8, D_INT = 16, D_SG = 32};

typedef struct _pid{
    float kp[5], kd[5], ki[5];
    float threshold[5];
    float erri ,err_ant2, err_ant1;
    float errd,errd_ant,errd_acum;
    float u, u_ant;
    float tant;
    float isat, osat, dt;
    int type;
    float N_filt;
} pid;

float computePid(pid* p, float error, float t, float h);
void resetPid(pid* p, uint32_t ti);
void initPid(pid* p, float kp, float ki, float kd,float ti, float N, float isat,float osat);
pid copyPid(pid *p);


#endif
