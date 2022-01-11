#include <bits/stdc++.h>
#include "ss.hpp"
using namespace std;

void updateFM(droneModel *dm, double in[4]){
	double bias[4] = {-4, 5, -3, -6};
	for(int i = 0 ; i < 4; i++){
		in[i] += bias[i]; 
		(dm->F[i]) = 9.81/1000*(-0.0026*in[i]*in[i]*in[i] + 0.4892*in[i]*in[i] - 4.2855*in[i] + 0.8182);
		(dm->M[i]) = 0.05*(dm->F[i]);
	}
}

void updateAPP(droneModel *dm){
	(dm->app[0]) = 1.3*(4.2*(dm->F[0]) - 4.2*(dm->F[1]) - 4.2*(dm->F[2]) + 4.2*(dm->F[3])) - 0.77*(dm->ap[1])*(dm->ap[2]);
	(dm->app[1]) = 4.1*(dm->F[2]) - 4.1*(dm->F[1]) - 4.1*(dm->F[0]) + 4.1*(dm->F[3]) + 1.2*(dm->ap[0])*(dm->ap[2]);
	(dm->app[2]) = 0.7*(5.8*(dm->M[1]) - 5.8*(dm->M[0]) - 5.8*(dm->M[2]) + 5.8*(dm->M[3])) - 0.02*(dm->ap[1])*(dm->ap[0]);
}

void updateXpp(droneModel *dm){
	(dm->rpp[0]) = 0.27*(sin((dm->a[0]))*sin((dm->a[2])) + cos((dm->a[0]))*cos((dm->a[2]))*sin((dm->a[1])))*((dm->F[0]) + (dm->F[1]) + (dm->F[2]) + (dm->F[3]));
	(dm->rpp[1]) =-0.27*(cos((dm->a[2]))*sin((dm->a[0])) - 1.0*cos((dm->a[0]))*sin((dm->a[1]))*sin((dm->a[2])))*((dm->F[0]) + (dm->F[1]) + (dm->F[2]) + (dm->F[3]));
	(dm->rpp[2]) = 0.27*cos((dm->a[1]))*cos((dm->a[0]))*((dm->F[0]) + (dm->F[1]) + (dm->F[2]) + (dm->F[3])) - 9.8;
}


void computeModel(droneModel *dm, double u[4], double dt){

		updateFM(dm, u);
		updateAPP(dm);
	
		for(int i = 0 ; i < 3; i++)
			(dm->ap[i]) += (dm->app[i])*dt;
		
		for(int i = 0 ; i < 3; i++)
			(dm->a[i]) += (dm->ap[i])*dt;

		updateXpp(dm);

		for(int i = 0 ; i < 3; i++)
			(dm->rp[i]) += (dm->rpp[i])*dt;

		for(int i = 0 ; i < 3; i++)
			(dm->r[i]) += (dm->rp[i])*dt;

		//printf("%f\t%f\t%f\t%f\n", (dm->a[0])*180/pi, (dm->a[1])*180/pi, (dm->a[2])*180/pi, (dm->r[2]));
}

void initModel(droneModel *dm){
	for(int i = 0 ; i < 4; i++){
		(dm->F[i]) = (dm->M[i]) = 0;
		if(i < 3){
			(dm->a[i])=0, (dm->ap[i])=0, (dm->app[i])=0,
			(dm->rpp[i])=0, (dm->rp[i])=0, (dm->r[i])=0;
		}
	}
}

droneModel copyModel(droneModel *dm){
	droneModel aux;
	for(int i = 0 ; i < 4; i++){
		aux.F[i] = (dm->F[i]);
		aux.M[i] = (dm->M[i]);
		if(i < 3){
	 		(aux.a[i])	= (dm->a[i]);
	 		(aux.ap[i]) = (dm->ap[i]);
	 		(aux.app[i]) = (dm->app[i]);
	 		(aux.rpp[i]) = (dm->rpp[i]);
	 		(aux.rp[i]) = (dm->rp[i]);
	 		(aux.r[i]) = (dm->r[i]);
		}
	}
	return aux;
}
