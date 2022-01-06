#include <bits/stdc++.h>
#include "ss.hpp"
using namespace std;

void updateU(droneModel *dm, double in[4]){
	for(int i = 0 ; i < 4; i++){
		(dm->U[i]) = in[i];
	}
}

void updateAPP(droneModel *dm){
	(dm->app[0]) = 15*(dm->U[0]) - 0.8*(dm->ap[1])*(dm->ap[2]);
	(dm->app[1]) = 12*(dm->U[1]) + 1.9*(dm->ap[0])*(dm->ap[2]);
	(dm->app[2]) = 7*(dm->U[2]) - 0.02*(dm->ap[1])*(dm->ap[0]);
}

void updateXpp(droneModel *dm){
	(dm->rpp[0]) = 0.29*(sin((dm->a[0]))*sin((dm->a[2])) + cos((dm->a[0]))*cos((dm->a[2]))*sin((dm->a[1])))*((dm->U[3]));
	(dm->rpp[1]) =-0.29*(cos((dm->a[2]))*sin((dm->a[0])) - 1.0*cos((dm->a[0]))*sin((dm->a[1]))*sin((dm->a[2])))*((dm->U[3]));
	(dm->rpp[2]) = 0.29*cos((dm->a[1]))*cos((dm->a[0]))*((dm->U[3])) - 9.8;
}


void computeModel(droneModel *dm, double u[4], double dt){

		updateU(dm, u);
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

		printf("%f\t%f\t%f\t%f\n", (dm->a[0])*180/pi, (dm->a[1])*180/pi, (dm->a[2])*180/pi, (dm->ap[0]));
}

void initModel(droneModel *dm){
	for(int i = 0 ; i < 4; i++){
		(dm->U[i]) = 0;
		if(i < 3){
			(dm->a[i])=0, (dm->ap[i])=0, (dm->app[i])=0,
			(dm->rpp[i])=0, (dm->rp[i])=0, (dm->r[i])=0;
		}
	}
}

droneModel copyModel(droneModel *dm){
	droneModel aux;
	for(int i = 0 ; i < 4; i++){
		aux.U[i] = (dm->U[i]);
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
