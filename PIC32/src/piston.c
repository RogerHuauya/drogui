#include "piston.h"

#define PPV 800
#define UPPER_LIMIT 0, 4
#define LOWER_LIMIT 2, 4

#define TIMER_VEL 2, 3
#define TIMER_ACCEL 3, 2

piston* my_p;

void setPiston(piston* p){
	my_p = p;
}

void initPistonStepper(int port_dir, int p_dir,int port_stp, int p_stp){
	
	initStepper(&my_p->s, port_dir, p_dir, port_stp, p_stp);
	initInterrupts(UPPER_LIMIT);
	initInterrupts(LOWER_LIMIT);
}

void initPistonVelTimer(int pre){
	initTimer(&my_p->velo, TIMER_VEL, pre);
	setTimerFrecuency(&my_p->velo,0);
	my_p -> vel_d = 0;
	my_p -> vel = 0;
}

void initPistonAccelTimer(int pre, double freq){
	initTimer(&my_p->accel, TIMER_ACCEL, pre);
	setTimerFrecuency(&my_p->accel, freq);
}

void setVelPiston(piston* p, double vel){
	if(vel < 0) setStepperDir(&(p->s), BWD), p -> dir = BWD;
	else setStepperDir(&(p->s), FWD), p -> dir = FWD;
	p -> vel_d = abs(vel);
}

void moveHome(piston* p){
	if(digitalRead(PRTD, 12) == 0){
		(p -> s).pos = 0;
	}
	else{
		(p -> s).pos = 10000; 
		while((p->s).pos > 0){
			setVelPiston(p, -200);
		}
	}
}

void stopPiston(piston* p){
	p->vel = p->vel_d = 0;
	setTimerFrecuency(&(p->velo), 0);
}

double getPos(piston *p){
	return 12.0*(p->s).pos/PPV;
}

double getVeld(){
	return my_p -> vel_d;
}

void EVAL(intInterrupt,UPPER_LIMIT){
	
	(my_p -> s).pos = (long long) 1.0 * PPV * MAXLEN / 12.0;
	stopPiston(my_p); 
	clearIntFlag(UPPER_LIMIT);
}


void EVAL(intInterrupt, LOWER_LIMIT){
	
	(my_p -> s).pos = 0;
	stopPiston(my_p);
	clearIntFlag(LOWER_LIMIT);
}

void EVAL(timerInterrupt, TIMER_ACCEL){

    if(my_p->vel != my_p->vel_d){

        if(my_p -> vel_d > my_p -> vel) my_p->vel += min(1,abs(my_p->vel_d - my_p->vel));
        else if(my_p -> vel_d < my_p -> vel) 
			my_p -> vel -= min(1, abs( my_p -> vel - my_p -> vel_d));
        
		setTimerFrecuency(&(my_p -> velo), (my_p -> vel)*PPV/30);
    }
    clearTimerFlag(&(my_p -> accel));
}

void EVAL(timerInterrupt, TIMER_VEL){
	bool up = getPos(my_p) >= MAXLEN  && (my_p -> dir == FWD);
	bool down = getPos(my_p) <= 0  && (my_p -> dir == BWD);
	if(up || down){
			clearTimerFlag(&(my_p -> velo));
	}
	else{
		halfStepStepper(&(my_p -> s));
		clearTimerFlag(&(my_p -> velo));
	}
}

