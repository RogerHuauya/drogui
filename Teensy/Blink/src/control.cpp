#include "..\headers\control.h"
#include <math.h>


float computePid(pid* p, float error, unsigned long t, float h){
    float kp = 1, kd = 1, ki = 1;


    p->dt = (t - p->tant)/1000000.0;
    p->tant = t;
    p->erri = max(min(p->erri + 0.5*(error+p->err_ant1)*p->dt,p->isat),-p->isat);
    
    //if( !( (error*p->u_ant > 0)  && abs(p->u_ant) == p-> osat)  ) 
    //    p->erri += 0.5*(error+p->err_ant1)*p->dt;
    //p->errd = errord;
    p->errd = (error - p->err_ant1)/(p->dt + 0.00000000001);
    
    p->err_ant2 = p->err_ant1;
    p->err_ant1 = error;

    if( p->type & D_INT ){
        p->errd = p->N_filt*(error - p->errd_acum);
        p->errd_acum += 0.5*(p->errd+p->errd_ant)*p->dt;
    }

    if(p->type & D_FILTER){
        p->errd = computeFilter(&(p->f), p->errd);
    }    

    //return max(min(p->kp[0]*error + p->ki[0]*p->erri + p->kd[0]*p->errd,p->osat),-p->osat);
    if(p->type & P2ID) error *= fabs(error); 
    
    if(p->type & INDEXED){
        if(h <= 60)         kp = p->kp[0], kd = p->kd[0], ki = p->ki[0];
        else if(h <= 70)    kp = p->kp[1], kd = p->kd[1], ki = p->ki[1];
        else if(h <= 80)    kp = p->kp[2], kd = p->kd[2], ki = p->ki[2];
        else if(h <= 90)    kp = p->kp[3], kd = p->kd[3], ki = p->ki[3];
        else if(h <= 100)   kp = p->kp[4], kd = p->kd[4], ki = p->ki[4];
    }
    else{kp = p->kp[0], kd = p->kd[0], ki = p->ki[0];}

    if(p->type & PIDABS){
        float newd = copysign(min( abs(kp*error), abs(kd*p->errd) ), p->errd);
        p->u = max(min(kp*error + ki*p->erri + newd, p->osat), -p->osat);
    }
    else{
        p->u = max(min(kp*error + ki*p->erri + kd*p->errd, p->osat), -p->osat);
    }
    p->u_ant = p->u;
    return p->u;
}


void resetPid(pid* p, unsigned long ti){
    p->tant = ti;
    p->erri = 0;
    p->err_ant1 = p->err_ant2 = 0;
    p->errd_acum = 0;
    p->u_ant = 0;
}

void initPid(pid* p, float kp, float kd, float ki,unsigned long ti, float N, float isat, float osat, int type){
    for(int i = 0; i < 5; i++){
        p->kp[i] = kp;
        p->kd[i] = kd;
        p->ki[i] = ki;
    }
    p->N_filt = N;
    p->errd_acum = 0;
    p->errd_ant = 0;
    p->u_ant = 0;
    p->tant = ti;
    p->osat = osat;
    p->type = type;
    p->isat = isat;
}

void initPid(pid* p, float kp, float kd, float ki,unsigned long ti, float N, float isat, float osat, int type,int n, float* a , float*b ){
    for(int i = 0; i < 5; i++){
        p->kp[i] = kp;
        p->kd[i] = kd;
        p->ki[i] = ki;
    }
    p->N_filt = N;
    p->errd_acum = 0;
    p->errd_ant = 0;
    p->u_ant = 0;
    p->tant = ti;
    p->isat = isat, 
    p->osat = osat;
    p->type = type;

    
    if(p->type & D_FILTER){
        initFilter(&(p->f), n, a, b);
    }
}
