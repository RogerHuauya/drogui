#include "control.hpp"

char buffct[50]="";

float computePid(pid* p, float error, float t, float h){

    float kp = 1, kd = 1, ki = 1;

    p->dt = (t - p->tant);
    p->tant = t;
    p->erri = fmax(fmin(p->erri + 0.5*(error+p->err_ant1)*p->dt,p->isat),-p->isat);

    //if( !( (error*p->u_ant > 0)  && abs(p->u_ant) == p-> osat)  )
    //    p->erri += 0.5*(error+p->err_ant1)*p->dt;
    //p->errd = errord;
	if(h>0)
	printf("errorant: %f %f", p->err_ant1, t);
    p->errd = (error - p->err_ant1)/(p->dt);

    p->err_ant2 = p->err_ant1;
    p->err_ant1 = error;

    //if( p->type & D_INT){
    //    p->errd = p->N_filt*(error - p->errd_acum);
    //    p->errd_acum += 0.5*(p->errd+p->errd_ant)*p->dt;
    //}

    //return max(min(p->kp[0]*error + p->ki[0]*p->erri + p->kd[0]*p->errd,p->osat),-p->osat);

    kp = p->kp[0], kd = p->kd[0], ki = p->ki[0];

	if(h>0)
	printf(" pid: %f\t%f\t%f\t", error, p->errd, p->erri);

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

void initPid(pid* p, float kp, float ki, float kd,float ti, float N, float isat,float osat){
    for(int i = 0; i < 5; i++){
        p->kp[i] = kp;
        p->kd[i] = kd;
        p->ki[i] = ki;
    }
    p->N_filt = N;
    p->errd_acum = 0;
    p->errd_ant = 0;
    p->err_ant1 = p->err_ant2 = 0;
    p->u_ant = 0;
    p->tant = ti;
    p->osat = osat;
    p->isat = isat;
}

pid copyPid(pid *p){
	pid aux; 
    aux.errd_acum = p->errd_acum;
    aux.errd_ant = p->errd_ant;
    aux.err_ant1 = p->err_ant1;
    aux.u_ant = p->u_ant;
	return aux;
}
