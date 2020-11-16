#include "control.h"
double computePid(pid* p, double error, double errord, unsigned long long t, double h){
    p->dt = (t - p->tant)/1000.0;
    p->tant = t;
    p->erri = max(min(p->erri + error*p->dt,p->isat),-p->isat);
    p->errd = errord;//(error - p->e_ant)/p->dt;
    p->e_ant = error;
    if(h <= 60)
        return max(min(p->kp[0]*error + p->ki[0]*p->erri + p->kd[0]*p->errd,p->osat),-p->osat);
    else if(h <= 70)
        return max(min(p->kp[1]*error + p->ki[1]*p->erri + p->kd[1]*p->errd,p->osat),-p->osat);
    else if(h <= 80)
        return max(min(p->kp[2]*error + p->ki[2]*p->erri + p->kd[2]*p->errd,p->osat),-p->osat);
    else if(h <= 90)
        return max(min(p->kp[3]*error + p->ki[3]*p->erri + p->kd[3]*p->errd,p->osat),-p->osat);
    else if(h <= 100)
        return max(min(p->kp[4]*error + p->ki[4]*p->erri + p->kd[4]*p->errd,p->osat),-p->osat);
}

void resetPid(pid* p, double ti){
    p->tant = ti;
    p->erri = 0;
    p->e_ant = 0;
}

void initPid(pid* p, double kp, double kd, double ki,double ti,double isat,double osat){
    for(int i = 0; i < 5; i++){
        p->kp[i] = kp;
        p->kd[i] = kd;
        p->ki[i] = ki;
    }
    p->tant = ti;
    p->isat = isat, p->osat = osat;
}