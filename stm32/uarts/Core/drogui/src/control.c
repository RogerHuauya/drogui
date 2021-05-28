#include "control.h"
#include <math.h>
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "serial.h"

char buffct[50]="";

float computePid(pid* p, float error, uint32_t t, float h){
    
    float kp = 1, kd = 1, ki = 1;

    p->dt = (t - p->tant)/1000000.0;
    p->tant = t;
    p->erri = fmax(fmin(p->erri + 0.5*(error+p->err_ant1)*p->dt,p->isat),-p->isat);
    
    //if( !( (error*p->u_ant > 0)  && abs(p->u_ant) == p-> osat)  ) 
    //    p->erri += 0.5*(error+p->err_ant1)*p->dt;
    //p->errd = errord;
    p->errd = (error - p->err_ant1)/(p->dt + EPS);
    
    p->err_ant2 = p->err_ant1;
    p->err_ant1 = error;
    
    if( p->type & D_INT){
        p->errd = p->N_filt*(error - p->errd_acum);
        p->errd_acum += 0.5*(p->errd+p->errd_ant)*p->dt;
    }

    if( p->type & D_SG){
        p->errd = computeSavGolDFilter(&(p->sgd), error);
        //if(h==50) serialPrintf("%f\t", p->errd);
    }

    if(p->type & D_FILTER){

        p->errd = computeFilter(&(p->f), p->errd);
        //if(h==50)serialPrintf("%f\n", p->errd);
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
        float newd = copysign(fmin( fabs(kp*error), fabs(kd*p->errd) ), p->errd);
        p->u = fmax(fmin(kp*error + ki*p->erri + newd, p->osat), -p->osat);
    }
    else{
        p->u = fmax(fmin(kp*error + ki*p->erri + kd*p->errd, p->osat), -p->osat);
    }
    p->u_ant = p->u;

   
    return p->u;
}


float computePidD(pid* p, float error, uint32_t t, float h, float dp){
    
    float kp = 1, kd = 1, ki = 1;

    p->dt = (t - p->tant)/1000000.0;
    p->tant = t;
    p->erri = fmax(fmin(p->erri + 0.5*(error+p->err_ant1)*p->dt,p->isat),-p->isat);
    
    //if( !( (error*p->u_ant > 0)  && abs(p->u_ant) == p-> osat)  ) 
    //    p->erri += 0.5*(error+p->err_ant1)*p->dt;
    //p->errd = errord;
    p->errd = dp;
    
    p->err_ant2 = p->err_ant1;
    p->err_ant1 = error;
    
    if( p->type & D_INT){
        p->errd = p->N_filt*(error - p->errd_acum);
        p->errd_acum += 0.5*(p->errd+p->errd_ant)*p->dt;
    }

    if( p->type & D_SG){
        p->errd = computeSavGolDFilter(&(p->sgd), error);
        //if(h==50) serialPrintf("%f\t", p->errd);
    }

    if(p->type & D_FILTER){

        p->errd = computeFilter(&(p->f), p->errd);
        //if(h==50)serialPrintf("%f\n", p->errd);
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
        float newd = copysign(fmin( fabs(kp*error), fabs(kd*p->errd) ), p->errd);
        p->u = fmax(fmin(kp*error + ki*p->erri + newd, p->osat), -p->osat);
    }
    else{
        p->u = fmax(fmin(kp*error + ki*p->erri + kd*p->errd, p->osat), -p->osat);
    }
    p->u_ant = p->u;

   
    return p->u;
}


void resetPid(pid* p, uint32_t ti){
    p->tant = ti;
    p->erri = 0;
    p->err_ant1 = p->err_ant2 = 0;
    p->errd_acum = 0;
    p->u_ant = 0;
}

void initPid(pid* p, float kp, float kd, float ki,uint32_t ti, float N, float isat, float osat, int type){
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
    initSavGolDFilter(&(p->sgd), 13);
}

void initPidFilter(pid* p, float kp, float kd, float ki,uint32_t ti, float N, float isat, float osat, int type,int n, float* a , float*b ){
    
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
    
    initSavGolDFilter(&(p->sgd), 13);
    
    if(p->type & D_FILTER){
        initFilter(&(p->f), n, a, b);
    }
}
