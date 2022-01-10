#include <bits/stdc++.h>
#include <time.h>
#include "matplotlibcpp.h"
#include "control.hpp"
#include "ss.hpp"
using namespace std;
namespace plt = matplotlibcpp;

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

}

double signFunc(double s){
	return 2/(1 + exp(-100*s))-1;
	//return (s>0?1:-1);
}

int main(){
	double dt = 0.001;
	initModel(&drogui);
	srand(time(NULL));
	double u[4];
	double erri[4]={0,0,0,0};
	double g[4]; 
	double f[4];
	double u_c[4];
	double u_d[4];
	double U[4];
	double err;
	double errd;
	double s;

	vector<double> plt1, plt2, plt3, plt4, plt5, plt6, plt7, plt8;
	printf("close all;\narr=[\n");

	for(double tim = dt ; tim < 10; tim += dt){
		double sp[4] = {0.1*sin(tim) ,0.1*cos(tim), pi/6, 1};
		{
			err = sp[0] - noise(drogui.a[0]);
			erri[0] += err*dt;
			errd = 0.1*cos(tim) - noise(drogui.ap[0]);

			s = errd * 0.1 + err;
			u_d[0] = 2*signFunc(s);

			f[0] = -0.77*(drogui.ap[2]*drogui.ap[1]);
			g[0] = 0.1;
			u_c[0] = (12*(errd) + 80*(err) + 50*erri[0]);

			U[0] = g[0]*(u_c[0] - f[0]) + u_d[0];
		}

		{	
			err = sp[1] - noise(drogui.a[1]);
			erri[1] += err*dt;
			errd = -0.1*sin(tim) - noise(drogui.ap[1]);

			s = errd * 0.1 + err;
			u_d[1] = 2*signFunc(s);

			f[1] = 0.78*(drogui.ap[0]*drogui.ap[2]);
			g[1] = 0.1;
			u_c[1] = (12*(errd) + 80*(err) +40*erri[1]);

			U[1] = g[1]*( u_c[1] - f[1])+ u_d[1];
		}

		{
			err = sp[2] - noise(drogui.a[2]);
			erri[2] += err*dt;
			errd = 0 - noise(drogui.ap[2]);

			s = errd * 0 + err;
			u_d[2] = 1*signFunc(s);

			f[2] = -0.011*(drogui.ap[1]*drogui.a[0]);
			g[2] = 0.1724;
			u_c[2] = (10*(errd) + 20*(err));

			U[2] = g[2]*( u_c[2] - f[2])+ u_d[2];
		}

		{	
			err = sp[3] - noise(drogui.r[2], 0.05);
			erri[3] += err*dt;
			errd = 0 - noise(drogui.rp[2], 0.01);

			s = errd * 0 + err;
			u_d[3] = 24*signFunc(s);

			f[3] = -9.81; 
			g[3] = 1.0/(0.29*cos(drogui.a[0])*cos(drogui.a[1]));
			u_c[3] = (8*(errd) + 0*err  + 4*erri[3]);

			U[3] = g[3]*( u_c[3] - f[3]) + u_d[3];
		}
		U2umotor(U, u);

	//	printf("%f\t%f\t%f\t%f\t", U[0], U[1], U[2], U[3]);
	//	printf("%f\t%f\t%f\t%f\t", u[0], u[1], u[2], u[3]);
		//printf("%f\t", signFunc(tim - 5));
		//printf("%f\t%f\t%f\t%f\t", sp[0]*180/pi, sp[1]*180/pi, sp[2]*180/pi, sp[3]);

		computeModel(&drogui, u, dt);
		plt1.push_back(sp[0]*180/pi);
		plt2.push_back(sp[1]*180/pi);
		plt3.push_back(sp[2]*180/pi);
		plt4.push_back(sp[3]);

		plt5.push_back(drogui.a[0]*180/pi);
		plt6.push_back(drogui.a[1]*180/pi);
		plt7.push_back(drogui.a[2]*180/pi);
		plt8.push_back(drogui.r[2]);

		
	}
	plt::figure(1);
	//plt::figure_size(1200,780);
	plt::plot(plt1);
	plt::plot(plt5);

	plt::figure(2);
	//plt::figure_size(1200,780);
	plt::plot(plt2);
	plt::plot(plt6);

	plt::figure(3);
	//plt::figure_size(1200,780);
	plt::plot(plt3);
	plt::plot(plt7);

	plt::figure(4);
	//plt::figure_size(1200,780);
	plt::plot(plt4);
	plt::plot(plt8);
	plt::show();	
}
