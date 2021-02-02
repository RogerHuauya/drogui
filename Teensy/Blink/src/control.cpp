#include "..\headers\control.h"
#include <math.h>

pid z_control;
pid x_control;
pid y_control;

pid roll_control;
pid pitch_control;
pid yaw_control;


double computePid(pid* p, double error, unsigned long long t, double h){
    double kp = 1, kd = 1, ki = 1;


    p->dt = (t - p->tant)/1000.0;
    p->tant = t;
    p->erri = max(min(p->erri + error*p->dt,p->isat),-p->isat);
    //p->errd = errord;
    p->errd = (error - p->err_ant2)/(2*p->dt);
    
    p->err_ant2 = p->err_ant1;
    p->err_ant1 = error;
    

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
        double newd = -copysign(min( abs(kp*error), abs(kd*p->errd) ), error);
        return max(min(kp*error + ki*p->erri + newd, p->osat), -p->osat);
    }
    else{
        return max(min(kp*error + ki*p->erri + kd*p->errd, p->osat), -p->osat);
    }

    return 0;
}


void resetPid(pid* p, double ti){
    p->tant = ti;
    p->erri = 0;
    p->err_ant1 = p->err_ant2 = 0;
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

double roll_const[5][3] = {{30, 0, 15}, {30, 300, 15}, {30, 300, 15}, {30, 300, 15}, {30, 300, 15}};
double pitch_const[5][3] = {{30, 0, 15}, {30, 300, 15}, {30, 300, 15}, {30, 300, 15}, {30, 300, 15}};

//double roll_const[5][3] = {{10, 10, 4}, {10, 10, 4}, {10, 10, 5}, {20, 25, 15}, {20, 25, 15}};
//double pitch_const[5][3] = {{10, 10, 4}, {10, 10, 4}, {10, 10, 5}, {20, 25, 15}, {20, 25, 15}};
double yaw_const[5][3] = {{0, 0, 0}, {0,0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

void initPidConstants(){
    
    initPid(&z_control, 0, 0, 0, 0, 1 , 15, NORMAL);
    initPid(&x_control, 0.25, 0, 0, 0, 1 , 0.09, NORMAL);
    initPid(&y_control, 0.25, 0, 0, 0, 1 , 0.09, NORMAL);
    
    initPid(&roll_control, 0, 0, 0, 0, 1 , 10000, NORMAL);
    initPid(&pitch_control, 0, 0, 0, 0, 1 , 10000, NORMAL);
    initPid(&yaw_control, 0, 0, 0, 0, 1 , 10000, NORMAL);
    
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
