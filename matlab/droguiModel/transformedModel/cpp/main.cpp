#include <bits/stdc++.h>
#include <time.h>
#include "control.hpp"
#include "ss_transformed.hpp"
using namespace std;

pid roll_ctrl, pitch_ctrl, yaw_ctrl, z_ctrl;
droneModel drogui;
double H, R, P, Y;

double angle_dif(double angle1, double angle2){
	if(angle1 > angle2){
		if((angle1 - angle2) > (2*pi - angle1 + angle2)) return -2*pi + angle1 - angle2;
		else return angle1 - angle2;
	}
	else{
		if((angle2 - angle1) > (2*pi - angle2 + angle1)) return 2*pi - angle2 + angle1;
		else return angle1 - angle2;
	}
}

double noise(double var, double amp = 5*pi/180){
	return var +  amp*(rand()%2000-1000)/1000.0 ;
}



int main(){
	double dt = 0.001;
	initModel(&drogui);
	srand(time(NULL));
	double u[4];
	double erri[4];
	double err;
	printf("close all;\narr=[\n");

	for(double tim = dt ; tim < 10; tim += dt){
		double sp[4] = {0.1*sin(tim) ,pi/6, pi/2, 1};

		err = sp[0] - noise(drogui.a[0]);
		erri[0] += err*dt;
		u[0] = 0.1*( (12*0.1*cos(tim) - 12*noise(drogui.ap[0]) + 200*(err) + 300*erri[0]) -  + 0.77*(drogui.ap[2]*drogui.ap[1]));
		
		err = sp[1] - noise(drogui.a[1]);
		erri[1] += err*dt;
		u[1] = 0.1*( (-12*noise(drogui.ap[1]) + 200*(err) +300*erri[1]) -  - 0.78*(drogui.ap[0]*drogui.ap[2]));
		
		u[2] = 0.1724*( (-50*noise(drogui.ap[2]) + 100*(sp[2] - noise(drogui.a[2]))) -  + 0.011*(drogui.ap[1]*drogui.a[0]));
		
		err = sp[3] - noise(drogui.r[2], 0.05);
		erri[3] += err*dt;
		u[3] = 1.0/(0.29*cos(drogui.a[0])*cos(drogui.a[1]))*( (-50*noise(drogui.rp[2], 0.01) + 100*(err) + 40*erri[3]) );

		printf("%f\t%f\t%f\t%f\t", sp[0]*180/pi, sp[1]*180/pi, sp[2]*180/pi, sp[3]);
		computeModel(&drogui, u, dt);
	}
	printf("];\n");
	printf("figure\n");
	printf("plot(arr(:,1))\n");
	printf("hold on\n");
	printf("plot(arr(:,5))\n");

	printf("figure\n");
	printf("plot(arr(:,2))\n");
	printf("hold on\n");
	printf("plot(arr(:,6))\n");

	printf("figure\n");
	printf("plot(arr(:,3))\n");
	printf("hold on\n");
	printf("plot(arr(:,7))\n");

	printf("figure\n");
	printf("plot(arr(:,4))\n");
	printf("hold on\n");
	printf("plot(arr(:,8))\n");
}
