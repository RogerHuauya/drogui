#include "control.h"
double computePid(pid* p, double error, double t){
    p->dt = (t - p->tant)/1000.0;
    p->tant = t;
    p->erri = max(min(p->erri + error*p->dt,p->isat),-p->isat);
    p->errd = (error - p->e_ant)/p->dt;
    p->e_ant = error;
    return max(min(p->kp*error + p->ki*p->erri + p->kd*p->errd,p->osat),-p->osat);
}

void resetPid(pid* p, double ti){
    p->tant = ti;
    p->erri = 0;
    p->e_ant = 0;
}

void initPid(pid* p, double kp, double kd, double ki,double ti,double isat,double osat){
    p->kp = kp, p->kd = kd;
    p->ki = ki, p->tant = ti;
    p->isat = isat, p->osat = osat;
}