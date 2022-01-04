#include <bits/stdc++.h>
#define pi 3.14159265359
#include "control.hpp"
#include "ss.hpp"
using namespace std;

pid roll_ctrl, pitch_ctrl, yaw_ctrl, z_ctrl;
droneModel drogui;
float H, R, P, Y;

float angle_dif(float angle1, float angle2){
	if(angle1 > angle2){
		if((angle1 - angle2) > (2*pi - angle1 + angle2)) return -2*pi + angle1 - angle2;
		else return angle1 - angle2;
	}
	else{
		if((angle2 - angle1) > (2*pi - angle2 + angle1)) return 2*pi - angle2 + angle1;
		else return angle1 - angle2;
	}
}


float cost(float kp, float ki, float kd){

	droneModel local = copyModel(&drogui);
	pid roll_, pitch_ctrl, yaw_ctrl, z_ctrl;
	float dt = 0.01;
	float u[4];
	float sp[4] = {0.1 ,pi/6, pi/2, 1};
	float J;
	float w[4] = {1, 1, 0.6, 0.2};

	initPid(&roll_ctrl, 10, 0, 2, 0, 50, 100000,1000000);
	initPid(&pitch_ctrl, 10, 0, 2, 0, 50, 10000,1000000);
	initPid(&yaw_ctrl, 20, 1, 10, 0, 50, 10000,1000000);
	initPid(&z_ctrl, 15, 8, 10, 0, 50, 10000,10000000);

	for(float tim = dt ; tim < 0.5; tim += dt){

		R = computePid(&roll_ctrl, sp[0] - local.a[0], tim, 0);
		P = computePid(&pitch_ctrl, sp[1] - local.a[1], tim, 0);
		Y = computePid(&yaw_ctrl, sp[2] - local.a[2], tim, 0);
		H = computePid(&z_ctrl, sp[3] - local.r[2], tim, 0) + 55;

		J += w[0]*roll_ctrl.err_ant1 + w[1]*pitch_ctrl.err_ant1 + \
			 w[2]*yaw_ctrl.err_ant1 + w[3]*z_ctrl.err_ant1;

		//R = 20;
		//P = 20;
		//Y = 20;
		//H = 100;

		u[0] = H +R -P -Y;
		u[1] = H -R -P +Y;
		u[2] = H -R +P -Y;
		u[3] = H +R +P +Y;

		for(int i = 0 ; i < 4 ; i++) u[i] = fmax(0, fmin(100, u[i]));

		computeModel(&local, u, dt);
		//printf(" U: %f\t%f\t%f\t%f\t", (u[0]), (u[1]), (u[2]), (u[2]));
		//printf(" U: %f\t%f\t%f\t%f\t", R, P, Y, H);

	}
	return J;
}


int main(){
	float dt = 0.001;
	initModel(&drogui);

	float u[4];
	float sp[4] = {0.1 ,pi/6, pi/2, 1};



	for(float tim = dt ; tim < 10; tim += dt){

		R = computePid(&roll_ctrl, sp[0] - drogui.a[0], tim, 0);
		P = computePid(&pitch_ctrl, sp[1] - drogui.a[1], tim, 0);
		Y = computePid(&yaw_ctrl, sp[2] - drogui.a[2], tim, 0);
		H = computePid(&z_ctrl, sp[3] - drogui.r[2], tim, 0) + 55;

		//R = 20;
		//P = 20;
		//Y = 20;
		//H = 100;

		u[0] = H +R -P -Y;
		u[1] = H -R -P +Y;
		u[2] = H -R +P -Y;
		u[3] = H +R +P +Y;

		for(int i = 0 ; i < 4 ; i++) u[i] = fmax(0, fmin(100, u[i]));

		computeModel(&drogui, u, dt);
		//printf(" U: %f\t%f\t%f\t%f\t", (u[0]), (u[1]), (u[2]), (u[2]));
		//printf(" U: %f\t%f\t%f\t%f\t", R, P, Y, H);

	}

}
