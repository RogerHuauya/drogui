#include <bits/stdc++.h>
#include <time.h>
#include "control.hpp"
#include "ss.hpp"
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
void U2umotor(double *U, double *umotor){
		double F[4];
		F[0] = U[0]*6.25E-1-U[1]*6.25E-1-U[2]*5+U[3]*2.5E-1;
		F[1] = U[0]*-6.25E-1-U[1]*6.25E-1+U[2]*5+U[3]*2.5E-1;
		F[2] = U[0]*-6.25E-1+U[1]*6.25E-1-U[2]*5+U[3]*2.5E-1;
		F[3] = U[0]*6.25E-1+U[1]*6.25E-1+U[2]*5+U[3]*2.5E-1;

		umotor[0] = F[0]*1.0833E+1-(F[0]*F[0])*6.34836E-1+(F[0]*F[0]*F[0])*1.86263E-2-6.6119E-2;
		umotor[1] = F[1]*1.0833E+1-(F[1]*F[1])*6.34836E-1+(F[1]*F[1]*F[1])*1.86263E-2-6.6119E-2;
		umotor[2] = F[2]*1.0833E+1-(F[2]*F[2])*6.34836E-1+(F[2]*F[2]*F[2])*1.86263E-2-6.6119E-2;
		umotor[3] = F[3]*1.0833E+1-(F[3]*F[3])*6.34836E-1+(F[3]*F[3]*F[3])*1.86263E-2-6.6119E-2;
		for(int i = 0 ; i < 4; i++) umotor[i] = fmin(fmax(0, umotor[i]),100);
		//umotor[0] +=50; 
		//umotor[1] +=50; 
		//umotor[2] +=50; 
		//umotor[3] +=50; 
}

int main(){
	double dt = 0.001;
	initModel(&drogui);
	srand(time(NULL));
	double u[4];
	double erri[4]={0,0,0,0};
	double U[4];
	double err;
	printf("close all;\narr=[\n");

	for(double tim = dt ; tim < 10; tim += dt){
		double sp[4] = {0.1*sin(tim) ,pi/6, pi/6, 1};

		err = sp[0] - noise(drogui.a[0]);
		erri[0] += err*dt;
		U[0] = 0.1*( (12*0.1*cos(tim) - 12*noise(drogui.ap[0]) + 60*(err) + 30*erri[0]) -  + 0.77*(drogui.ap[2]*drogui.ap[1]));
		
		err = sp[1] - noise(drogui.a[1]);
		erri[1] += err*dt;
		U[1] = 0.1*( (-12*noise(drogui.ap[1]) + 80*(err) +30*erri[1]) -  - 0.78*(drogui.ap[0]*drogui.ap[2]));
		
		U[2] = 0.1724*( (-10*noise(drogui.ap[2]) + 20*(sp[2] - noise(drogui.a[2]))) -  + 0.011*(drogui.ap[1]*drogui.a[0]));
		
		err = sp[3] - noise(drogui.r[2], 0.05);
		erri[3] += err*dt;
		U[3] = 1.0/(0.29*cos(drogui.a[0])*cos(drogui.a[1]))*( (-8*noise(drogui.rp[2], 0.01) + 8*(err) + 4*erri[3]) );
		U[3] += 30;

		U2umotor(U, u);

	//	printf("%f\t%f\t%f\t%f\t", U[0], U[1], U[2], U[3]);
	//	printf("%f\t%f\t%f\t%f\t", u[0], u[1], u[2], u[3]);
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
