#include "macros.h"
#ifdef KALMANPV
#include "kalmanPV.h"

#define GRAVITY 1


void setKalmanTsImu(kalmanPV* kpv, float ts){
	kpv->Ts = ts;
}

void setKalmanTsGps(kalmanPV* kpv, float ts){
	kpv->N = (kpv->Ts)/ts;
}

void initMatGlobal(kalmanPV* kpv){
	matInit(&(kpv->Rq), 3, 3);
	matInit(&(kpv->u), 3, 1);
	matInit(&(kpv->s), 3, 1);
	matInit(&(kpv->p), 3, 1);
	matInit(&(kpv->v), 3, 1);
	matInit(&(kpv->bias_p), 3, 1);
	matInit(&(kpv->bias_u), 3, 1);
	matInit(&(kpv->bias_v), 3, 1);
	matInit(&(kpv->ye), 4, 1);
	matInit(&(kpv->p_gps), 2, 1);
	matInit(&(kpv->v_gps), 2, 1);
	matInit(&(kpv->KalmanGain), 9, 4);
	matInit(&(kpv->delta), 9, 1);

	matInit(&(kpv->Rc), 4, 4);
	for(int i = 0; i < 4;  i++) setMatVal(&(kpv->Rc),i,i,0.1);

	matInit(&(kpv->Q12), 6, 6);
	for(int i = 0; i < 6;  i++) setMatVal(&(kpv->Q12),i,i,0.1);

	matInit(&(kpv->Pm), 9, 9);
	for(int i = 0; i < 9;  i++) setMatVal(&(kpv->Pm),i,i,1);

	matInit(&(kpv->Fm), 9, 9);
	for(int i = 0; i < 9 ; i++) setMatVal(&(kpv->Fm), i, i, 1);
	for(int i = 0; i < 3; i++) setMatVal(&(kpv->Fm), i, i+3, kpv->Ts);

	matInit(&(kpv->Hm), 4, 9);
	for(int i = 0; i < 2; i++) {
		setMatVal(&(kpv->Hm), i, i,1);
		setMatVal(&(kpv->Hm), i+2, i+3,1);
	}
	matInit(&(kpv->Gm),9,6);
	for( int i = 6;  i < 9; i++ ) setMatVal(&(kpv->Gm), i, i-3,1);

}

void kynematics(kalmanPV *kpv){
	mat aux1, aux2;

	matInit(&aux1, (kpv->p).row, (kpv->p).col);
	matScale(&aux1, &(kpv->v), (kpv->Ts));

	matInit(&aux2, (kpv->p).row, (kpv->p).col);
	matMult(&aux2, &(kpv->Rq), &(kpv->u));

	setMatVal(&aux2, 2, 0, getMatVal(&aux2, 2, 0) - 9.81);

	matScale(&aux2, &aux2, (kpv->Ts));

	matAdd(&(kpv->v), &(kpv->v), &aux2);

	matScale(&aux2, &aux2, (kpv->Ts)/2);

	matAdd(&(kpv->p), &(kpv->p), &aux1);
	matAdd(&(kpv->p), &(kpv->p), &aux2);


	matDestruct(&aux1);
	matDestruct(&aux2);
}


void getMatFm(kalmanPV *kpv){
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			setMatVal(&(kpv->Fm), i + 3 ,j + 6, (kpv->Ts)*getMatVal(&(kpv->Rq), i, j));
}

void getMatGm(kalmanPV *kpv){
	for( int i = 0;  i < 3; i++ ){
		for( int j = 0; j < 3; j++)
			setMatVal(&(kpv->Gm), i+3, j, (kpv->Ts)*getMatVal(&(kpv->Rq), i,j));
	}
}

void UpdatePm(kalmanPV *kpv){

	mat aux1,aux2,aux3,aux4,aux5;
	matInit(&aux1, 9, 9);
	matInit(&aux2, 6, 9);
	matInit(&aux3, 9, 9);
	matInit(&aux4, 9, 9);
	matInit(&aux5, 9, 6);

	getMatFm(kpv);
	getMatGm(kpv);

	matTrans(&aux1,&(kpv->Fm));
	matTrans(&aux2,&(kpv->Gm));

	matMult(&aux4,&(kpv->Fm),&(kpv->Pm));
	matMult(&(kpv->Pm),&aux4,&aux1);

	matMult(&aux5,&(kpv->Gm),&(kpv->Q12));
	matMult(&aux3,&aux5,&aux2);
	matAdd(&(kpv->Pm), &(kpv->Pm), &aux3);

	matDestruct(&aux1);
	matDestruct(&aux2);
	matDestruct(&aux3);
	matDestruct(&aux4);
	matDestruct(&aux5);
}
void getKalmanGain(kalmanPV *kpv){

	mat aux1, aux2, aux3,aux4;


	matInit(&aux1, 4, 4);
	matInit(&aux2, 9, 4);
	matInit(&aux3, 9, 4);
	matInit(&aux4, 4, 4);

	matTrans(&aux3, &(kpv->Hm));
	matMult(&aux2, &(kpv->Pm), &aux3);

	matMult(&aux1, &(kpv->Hm), &aux2);
	matAdd(&aux1, &aux1, &(kpv->Rc));
	matInv(&aux4, &aux1);

	matMult(&(kpv->KalmanGain), &aux2, &aux4);

	matDestruct(&aux1);
	matDestruct(&aux2);
	matDestruct(&aux3);
	matDestruct(&aux4);
}

void UpdatePmCovGPS(kalmanPV *kpv){
	mat aux1,aux2;
	matInit(&aux1, 9, 9);
	matInit(&aux2, 9, 9);
	matMult(&aux1, &(kpv->KalmanGain), &(kpv->Hm));
	matScale(&aux1, &aux1, -1);
	for(int i = 0; i < 9; i++)
		setMatVal(&aux1, i , i, 1 + getMatVal(&aux1, i , i));


	matMult(&aux2, &aux1, &(kpv->Pm));
	matCopy(&(kpv->Pm), &aux2);

	matDestruct(&aux1);
	matDestruct(&aux2);
}

void getBias(kalmanPV *kpv){


	//matSubs(&ye, &p_gps,  &p);
	setMatVal(&(kpv->ye), 0, 0, getMatVal(&(kpv->p_gps), 0, 0) - getMatVal(&(kpv->p), 0, 0));
	setMatVal(&(kpv->ye), 1, 0, getMatVal(&(kpv->p_gps), 1, 0) - getMatVal(&(kpv->p), 1, 0));
	setMatVal(&(kpv->ye), 2, 0, getMatVal(&(kpv->v_gps), 0, 0) - getMatVal(&(kpv->v), 0, 0));
	setMatVal(&(kpv->ye), 3, 0, getMatVal(&(kpv->v_gps), 1, 0) - getMatVal(&(kpv->v), 1, 0));

	matMult(&(kpv->delta), &(kpv->KalmanGain), &(kpv->ye));

	setMatVal(&(kpv->delta), 6, 0, getMatVal(&(kpv->delta), 6, 0) + getMatVal(&(kpv->bias_u), 0, 0));
	setMatVal(&(kpv->delta), 7, 0, getMatVal(&(kpv->delta), 7, 0) + getMatVal(&(kpv->bias_u), 1, 0));
	setMatVal(&(kpv->delta), 8, 0, getMatVal(&(kpv->delta), 8, 0) + getMatVal(&(kpv->bias_u), 2, 0));

	setMatVal(&(kpv->bias_p), 0, 0, getMatVal(&(kpv->delta), 0, 0));
	setMatVal(&(kpv->bias_p), 1, 0, getMatVal(&(kpv->delta), 1, 0));
	setMatVal(&(kpv->bias_p), 2, 0, getMatVal(&(kpv->delta), 2, 0));

	setMatVal(&(kpv->bias_v), 0, 0, getMatVal(&(kpv->delta), 3, 0));
	setMatVal(&(kpv->bias_v), 1, 0, getMatVal(&(kpv->delta), 4, 0));
	setMatVal(&(kpv->bias_v), 2, 0, getMatVal(&(kpv->delta), 5, 0));
}

void kalmanUpdateIMU(kalmanPV *kpv, float ax, float ay, float az,float roll, float pitch, float yaw){

	rpyToR(&(kpv->Rq), roll, pitch, yaw);
	setMatVal(&(kpv->u), 0, 0, ax*GRAVITY);
	setMatVal(&(kpv->u), 1, 0, ay*GRAVITY);
	setMatVal(&(kpv->u), 2, 0, az*GRAVITY);
	//matAdd(&u, &s, &bias_u);
	kynematics(kpv);
	UpdatePm(kpv);
}

void kalmanUpdateGPS(kalmanPV *kpv,float x_gps, float y_gps, float vx_gps, float vy_gps){

	setMatVal(&(kpv->p_gps), 0, 0, x_gps);
	setMatVal(&(kpv->p_gps), 1, 0, y_gps);

	setMatVal(&(kpv->v_gps), 0, 0, vx_gps);
	setMatVal(&(kpv->v_gps), 1, 0, vy_gps);

	getKalmanGain(kpv);

	getBias(kpv);

	UpdatePmCovGPS(kpv);

	matAdd(&(kpv->v), &(kpv->v), &(kpv->bias_v));
	matAdd(&(kpv->p), &(kpv->p), &(kpv->bias_p));
}



void getPosition(kalmanPV *kpv, float *x, float *y){
	*x = getMatVal(&(kpv->p), 0, 0);
	*y = getMatVal(&(kpv->p), 1, 0);
}

void getVelocity(kalmanPV *kpv, float *xp, float *yp){
	*xp = getMatVal(&(kpv->v), 0, 0);
	*yp = getMatVal(&(kpv->v), 1, 0);
}

void clearKalman(kalmanPV *kpv){

	for(int i = 0 ; i < 3 ; i++){
		setMatVal(&(kpv->p), i, 0, 0);
		setMatVal(&(kpv->v), i, 0, 0);
		setMatVal(&(kpv->bias_p), i, 0, 0);
		setMatVal(&(kpv->bias_v), i, 0, 0);
	}
	for(int i = 0; i < 9;  i++){
		for(int j = 0; j < 9; j++) setMatVal(&(kpv->Pm), i, j, 0);
		for(int j = 0; j < 4; j++) setMatVal(&(kpv->KalmanGain), i, j, 0);

		setMatVal(&(kpv->delta), i, 0, 0);
		setMatVal(&(kpv->Pm),i,i,0.1);
	}

}
#endif
