#include <bits/stdc++.h>
using namespace std;

float a[3], ap[3], app[3], rpp[3], rp[3], r[3];
float F[4], M[4];
float u[4];


void updateFM(float in[4]){
	for(int i = 0 ; i < 4; i++){
		F[i] = 9.81/1000*(-0.0026*in[i]*in[i]*in[i] + 0.4892*in[i]*in[i] - 4.2855*in[i] + 0.8182);
		M[i] = 5e-5*F[i]*( (i%2 == 0) ? -1 : 1);
	}
}

void updateAPP(float ap[3], float F[4],float M[4]){
	app[0] = 4.2*F[0] - 4.2*F[1] - 4.2*F[2] + 4.2*F[3] - 0.77*ap[1]*ap[2];
	app[1] = 4.1*F[2] - 4.1*F[1] - 4.1*F[0] + 4.1*F[3] + 0.78*ap[0]*ap[2];
	app[2] = 5.8*M[1] - 5.8*M[0] - 5.8*M[2] + 5.8*M[3] - 0.011*ap[1]*ap[0];
}

void updateXpp(float a[3], float F[4]){
	rpp[0] = 0.29*(sin(a[0])*sin(a[2]) + cos(a[0])*cos(a[2])*sin(a[1]))*(F[0] + F[1] + F[2] + F[3]);
	rpp[1] =-0.29*(cos(a[2])*sin(a[0]) - 1.0*cos(a[0])*sin(a[1])*sin(a[2]))*(F[0] + F[1] + F[2] + F[3]);
	rpp[2] = 0.29*cos(a[1])*cos(a[0])*(F[0] + F[1] + F[2] + F[3]) - 9.8;
}




int main(){
	u[0] = u[1] = 57.05;
	u[2] = u[3] = 57;
	float dt = 0.01;

	for(float time = 0; time < 10; time += dt){
		updateFM(u);
		updateAPP(ap, F, M);
	
		for(int i = 0 ; i < 3; i++)
			ap[i] += app[i]*dt;
		
		for(int i = 0 ; i < 3; i++)
			a[i] += ap[i]*dt;

		updateXpp(a, F);

		/*
		for(int i = 0 ; i < 3; i++)
			rp[i] += rpp[i]*dt;

		for(int i = 0 ; i < 3; i++)
			r[i] += rp[i]*dt;

		*/
		printf("%f\t%f\t%f\n", a[0], a[1], a[2]);
	}


}
