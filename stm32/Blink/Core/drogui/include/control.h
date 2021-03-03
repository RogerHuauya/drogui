#ifndef CONTROL_H
#define CONTROL_H

#include "utils.h"
#include "filter.h"


enum TYPES_PID {NORMAL = 0, P2ID = 1, INDEXED = 2, PIDABS = 4, D_FILTER = 8, D_INT = 16};
typedef struct _pid{
    float kp[5], kd[5], ki[5];
    float threshold[5];
    float erri ,err_ant2, err_ant1;
    float errd,errd_ant,errd_acum;
    float u, u_ant;
    uint32_t tant;
    float isat, osat, dt;
    int type;
    filter f;
    float N_filt;
} pid;

float computePid(pid* p, float error, uint32_t t, float h);
void resetPid(pid* p, uint32_t ti);

void initPid(pid* p, float kp, float kd, float ki,uint32_t ti, float N, float isat,float osat, int tipo );
void initPidFilter(pid* p, float kp, float kd, float ki,uint32_t ti, float N, float isat,float osat,int tipo, int n, float*a, float*b);


#endif
