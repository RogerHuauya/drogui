#include "..\headers\control.h"
#include <math.h>

pid z_control;
pid x_control;
pid y_control;

pid roll_control;
pid pitch_control;
pid yaw_control;


double computePid(pid* p, double error, unsigned long long t, double h){
    p->dt = (t - p->tant)/1000.0;
    p->tant = t;
    p->erri = max(min(p->erri + error*p->dt,p->isat),-p->isat);
    //p->errd = errord;
    p->errd = (error - p->e_ant)/p->dt;
    p->e_ant = error;
    //return max(min(p->kp[0]*error + p->ki[0]*p->erri + p->kd[0]*p->errd,p->osat),-p->osat);
    if(p->type & P2ID) error *= fabs(error); 
    
    if(p->type & INDEXED){
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
    else{
        return max(min(p->kp[0]*error + p->ki[0]*p->erri + p->kd[0]*p->errd,p->osat),-p->osat);    
    }
    return 0;
}


void resetPid(pid* p, double ti){
    p->tant = ti;
    p->erri = 0;
    p->e_ant = 0;
}

void initPid(pid* p, double kp, double kd, double ki,double ti,double isat,double osat, int type){
    for(int i = 0; i < 5; i++){
        p->kp[i] = kp;
        p->kd[i] = kd;
        p->ki[i] = ki;
    }
    p->tant = ti;
    p->isat = isat, p->osat = osat;
    p->type = type;
}


double roll_const[5][3] = {{25, 25, 10}, {25, 25, 10}, {20, 25, 15}, {20, 25, 15}, {20, 25, 15}};
double pitch_const[5][3] = {{25, 25, 10}, {25, 25, 10}, {20, 25, 15}, {20, 25, 15}, {20, 25, 15}};
double yaw_const[5][3] = {{0, 0, 0}, {0,0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

void initPidConstants(){
    
    initPid(&z_control, 0, 0, 0, 0, 10 , 100, P2ID);
    initPid(&x_control, 0.25, 0, 0, 0, 10 , 100, NORMAL);
    initPid(&y_control, 0.25, 0, 0, 0, 10 , 100, NORMAL);
    
    initPid(&roll_control, 0, 0, 0, 0, 1 , 100, INDEXED);
    initPid(&pitch_control, 0, 0, 0, 0, 1 , 100, INDEXED);
    initPid(&yaw_control, 0, 0, 0, 0, 1 , 100, INDEXED);
    
    for(int i = 0; i < 5; i ++){
        roll_control.kp[i] = roll_const[i][0];
        roll_control.ki[i] = roll_const[i][1];
        roll_control.kd[i] = roll_const[i][2];
    }
    for(int i = 0; i < 5; i ++){
        pitch_control.kp[i] = pitch_const[i][0];
        pitch_control.ki[i] = pitch_const[i][1];
        pitch_control.kd[i] = pitch_const[i][2];
    }
    for(int i = 0; i < 5; i ++){
        yaw_control.kp[i] = yaw_const[i][0];
        yaw_control.ki[i] = yaw_const[i][1];
        yaw_control.kd[i] = yaw_const[i][2];
    }

}
