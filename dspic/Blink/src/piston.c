#include "piston.h"


void initPistonStepper(piston *p, int port, int p_dir, int p_stp){
	initStepper(&p->s , port, p_dir, p_stp);
}

void initPistonVelTimer(piston *p, int n, int pre, int post){
	initTimer(&p->velo, n, pre, post);
	setTimerFrecuency(&p->velo,0);
	p -> vel_d = 0;
	p -> vel = 0;
}

void initPistonAccelTimer(piston *p, int n, int pre, int post, double freq){
	initTimer(&p->accel, n, pre, post);
	setTimerFrecuency(&p->accel, freq);
}

void setVelPiston(piston* p, double vel){
	p -> vel_d = vel;
}

void moveHome(piston* p){
	setStepperDir(&p->s, 0);
	while(true){
		double dis = analogRead(p -> p_dis);
		if(dis <= HOME) break;
		setVelPiston(p, dis*2 + 5);
	}
	stopPiston(p);
}

void stopPiston(piston* p){
	setVelPiston(p, 0);
}



/*
	v/60 (rps)
	
	3200p/vuelta * v/60 (vuelta / s)
	r*v*160/3 p/s




*/