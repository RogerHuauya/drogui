#include "macros.h"
#ifdef KALMANP
#include "kalmanP.h"

#define GRAVITY 1


void setKalmanTsImu(kalmanP* kp, float ts){
	kp->Ts = ts;
}

void setKalmanTsGps(kalmanP* kp, float ts){
	kp->N = (kp->Ts)/ts;
}

void initMatGlobal(kalmanP* kp){
	matInit(&(kp->Rq), 3, 3);
	matInit(&(kp->u), 3, 1);
	matInit(&(kp->s), 3, 1);
	matInit(&(kp->p), 3, 1);
	matInit(&(kp->v), 3, 1);
	matInit(&(kp->bias_p), 3, 1);
	matInit(&(kp->bias_u), 3, 1);
	matInit(&(kp->bias_v), 3, 1);
	matInit(&(kp->ye), 4, 1);
	matInit(&(kp->p_gps), 2, 1);
	matInit(&(kp->v_gps), 2, 1);
	matInit(&(kp->KalmanGain), 9, 4);
	matInit(&(kp->delta), 9, 1);

	matInit(&(kp->Rc), 4, 4);
	for(int i = 0; i < 4;  i++) setMatVal(&(kp->Rc),i,i,0.1);

	matInit(&(kp->Q12), 6, 6);
	for(int i = 0; i < 6;  i++) setMatVal(&(kp->Q12),i,i,0.1);

	matInit(&(kp->Pm), 9, 9);
	for(int i = 0; i < 9;  i++) setMatVal(&(kp->Pm),i,i,1);

	matInit(&(kp->Fm), 9, 9);
	for(int i = 0; i < 9 ; i++) setMatVal(&(kp->Fm), i, i, 1);
	for(int i = 0; i < 3; i++) setMatVal(&(kp->Fm), i, i+3, kp->Ts);

	matInit(&(kp->Hm), 4, 9);
	for(int i = 0; i < 2; i++) {
		setMatVal(&(kp->Hm), i, i,1);
		setMatVal(&(kp->Hm), i+2, i+3,1);
	}
	matInit(&(kp->Gm),9,6);
	for( int i = 6;  i < 9; i++ ) setMatVal(&(kp->Gm), i, i-3,1);

}

void kynematics(kalmanP *kp){
	mat aux1, aux2;

	matInit(&aux1, (kp->p).row, (kp->p).col);
	matScale(&aux1, &(kp->v), (kp->Ts));

	matInit(&aux2, (kp->p).row, (kp->p).col);
	matMult(&aux2, &(kp->Rq), &(kp->u));

	setMatVal(&aux2, 2, 0, getMatVal(&aux2, 2, 0) - 9.81);

	matScale(&aux2, &aux2, (kp->Ts));

	matAdd(&(kp->v), &(kp->v), &aux2);

	matScale(&aux2, &aux2, (kp->Ts)/2);

	matAdd(&(kp->p), &(kp->p), &aux1);
	matAdd(&(kp->p), &(kp->p), &aux2);


	matDestruct(&aux1);
	matDestruct(&aux2);
}


void getMatFm(kalmanP *kp){
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			setMatVal(&(kp->Fm), i + 3 ,j + 6, (kp->Ts)*getMatVal(&(kp->Rq), i, j));
}

void getMatGm(kalmanP *kp){
	for( int i = 0;  i < 3; i++ ){
		for( int j = 0; j < 3; j++)
			setMatVal(&(kp->Gm), i+3, j, (kp->Ts)*getMatVal(&(kp->Rq), i,j));
	}
}

void UpdatePm(kalmanP *kp){

	mat aux1,aux2,aux3,aux4,aux5;
	matInit(&aux1, 9, 9);
	matInit(&aux2, 6, 9);
	matInit(&aux3, 9, 9);
	matInit(&aux4, 9, 9);
	matInit(&aux5, 9, 6);

	getMatFm(kp);
	getMatGm(kp);

	matTrans(&aux1,&(kp->Fm));
	matTrans(&aux2,&(kp->Gm));

	matMult(&aux4,&(kp->Fm),&(kp->Pm));
	matMult(&(kp->Pm),&aux4,&aux1);

	matMult(&aux5,&(kp->Gm),&(kp->Q12));
	matMult(&aux3,&aux5,&aux2);
	matAdd(&(kp->Pm), &(kp->Pm), &aux3);

	matDestruct(&aux1);
	matDestruct(&aux2);
	matDestruct(&aux3);
	matDestruct(&aux4);
	matDestruct(&aux5);
}
void getKalmanGain(kalmanP *kp){

	mat aux1, aux2, aux3,aux4;


	matInit(&aux1, 4, 4);
	matInit(&aux2, 9, 4);
	matInit(&aux3, 9, 4);
	matInit(&aux4, 4, 4);

	matTrans(&aux3, &(kp->Hm));
	matMult(&aux2, &(kp->Pm), &aux3);

	matMult(&aux1, &(kp->Hm), &aux2);
	matAdd(&aux1, &aux1, &(kp->Rc));
	matInv(&aux4, &aux1);

	matMult(&(kp->KalmanGain), &aux2, &aux4);

	matDestruct(&aux1);
	matDestruct(&aux2);
	matDestruct(&aux3);
	matDestruct(&aux4);
}

void UpdatePmCovGPS(kalmanP *kp){
	mat aux1,aux2;
	matInit(&aux1, 9, 9);
	matInit(&aux2, 9, 9);
	matMult(&aux1, &(kp->KalmanGain), &(kp->Hm));
	matScale(&aux1, &aux1, -1);
	for(int i = 0; i < 9; i++)
		setMatVal(&aux1, i , i, 1 + getMatVal(&aux1, i , i));


	matMult(&aux2, &aux1, &(kp->Pm));
	matCopy(&(kp->Pm), &aux2);

	matDestruct(&aux1);
	matDestruct(&aux2);
}

void getBias(kalmanP *kp){


	//matSubs(&ye, &p_gps,  &p);
	setMatVal(&(kp->ye), 0, 0, getMatVal(&(kp->p_gps), 0, 0) - getMatVal(&(kp->p), 0, 0));
	setMatVal(&(kp->ye), 1, 0, getMatVal(&(kp->p_gps), 1, 0) - getMatVal(&(kp->p), 1, 0));
	setMatVal(&(kp->ye), 2, 0, getMatVal(&(kp->v_gps), 0, 0) - getMatVal(&(kp->v), 0, 0));
	setMatVal(&(kp->ye), 3, 0, getMatVal(&(kp->v_gps), 1, 0) - getMatVal(&(kp->v), 1, 0));

	matMult(&(kp->delta), &(kp->KalmanGain), &(kp->ye));

	setMatVal(&(kp->delta), 6, 0, getMatVal(&(kp->delta), 6, 0) + getMatVal(&(kp->bias_u), 0, 0));
	setMatVal(&(kp->delta), 7, 0, getMatVal(&(kp->delta), 7, 0) + getMatVal(&(kp->bias_u), 1, 0));
	setMatVal(&(kp->delta), 8, 0, getMatVal(&(kp->delta), 8, 0) + getMatVal(&(kp->bias_u), 2, 0));

	setMatVal(&(kp->bias_p), 0, 0, getMatVal(&(kp->delta), 0, 0));
	setMatVal(&(kp->bias_p), 1, 0, getMatVal(&(kp->delta), 1, 0));
	setMatVal(&(kp->bias_p), 2, 0, getMatVal(&(kp->delta), 2, 0));

	setMatVal(&(kp->bias_v), 0, 0, getMatVal(&(kp->delta), 3, 0));
	setMatVal(&(kp->bias_v), 1, 0, getMatVal(&(kp->delta), 4, 0));
	setMatVal(&(kp->bias_v), 2, 0, getMatVal(&(kp->delta), 5, 0));
}

void kalmanUpdateIMU(kalmanP *kp, float ax, float ay, float az,float roll, float pitch, float yaw){

	rpyToR(&(kp->Rq), roll, pitch, yaw);
	setMatVal(&(kp->u), 0, 0, ax*GRAVITY);
	setMatVal(&(kp->u), 1, 0, ay*GRAVITY);
	setMatVal(&(kp->u), 2, 0, az*GRAVITY);
	//matAdd(&u, &s, &bias_u);
	kynematics(kp);
	UpdatePm(kp);
}

void kalmanUpdateGPS(kalmanP *kp,float x_gps, float y_gps, float vx_gps, float vy_gps){

	setMatVal(&(kp->p_gps), 0, 0, x_gps);
	setMatVal(&(kp->p_gps), 1, 0, y_gps);

	setMatVal(&(kp->v_gps), 0, 0, vx_gps);
	setMatVal(&(kp->v_gps), 1, 0, vy_gps);

	getKalmanGain(kp);

	getBias(kp);

	UpdatePmCovGPS(kp);

	matAdd(&(kp->v), &(kp->v), &(kp->bias_v));
	matAdd(&(kp->p), &(kp->p), &(kp->bias_p));
}



void getPosition(kalmanP *kp, float *x, float *y){
	*x = getMatVal(&(kp->p), 0, 0);
	*y = getMatVal(&(kp->p), 1, 0);
}

void getVelocity(kalmanP *kp, float *xp, float *yp){
	*xp = getMatVal(&(kp->v), 0, 0);
	*yp = getMatVal(&(kp->v), 1, 0);
}

void clearKalman(kalmanP *kp){

	for(int i = 0 ; i < 3 ; i++){
		setMatVal(&(kp->p), i, 0, 0);
		setMatVal(&(kp->v), i, 0, 0);
		setMatVal(&(kp->bias_p), i, 0, 0);
		setMatVal(&(kp->bias_v), i, 0, 0);
	}
	for(int i = 0; i < 9;  i++){
		for(int j = 0; j < 9; j++) setMatVal(&(kp->Pm), i, j, 0);
		for(int j = 0; j < 4; j++) setMatVal(&(kp->KalmanGain), i, j, 0);

		setMatVal(&(kp->delta), i, 0, 0);
		setMatVal(&(kp->Pm),i,i,0.1);
	}

}
#endif
