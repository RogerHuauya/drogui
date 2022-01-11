#include "macros.h"
#include "tkf.h"
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "matlib.h"
#include "serial.h"

#define GRAVITY 9.81

void initTkf(tkf *t, float Ts){

	t->Ts = Ts;

	matInit( &(t->q), 4, 1);
	setMatVal( &(t->q), 0, 0, 1 );

	matInit( &(t->P), 4, 4);
	for(int i = 0; i < 4;  i++)
		for(int j = 0; j < 4;  j++) setMatVal(&(t->P),i,j,0.001);

	matInit( &(t->Q), 4, 4);
	for(int i = 0; i < 4;  i++) setMatVal( &(t->Q),i,i,0.000001);

	eyeMat(&(t->V1), 3);
	eyeMat(&(t->V2), 3);

	matInit( &(t->R1), 3, 3);
	for(int i = 0; i < 3;  i++) setMatVal( &(t->R1),i,i, 1.2*(i+1));

	matInit( &(t->R2), 3, 3);
	setMatVal( &(t->R2), 0, 0, 0.5);
	setMatVal( &(t->R2), 1, 1, 0.5*2);
	setMatVal( &(t->R2), 2, 2, 0.5);
	//for(int i = 0; i < 3;  i++) setMatVal( &(t->R2),i,i, 0.5*(i+1));
}



void tkfUpdate(tkf *t, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {

	mat a341, a311, a44I, a431, a432, a331, a332, a333;
	mat a441, a442, a443, a411;

	matInit(&a441, 4, 4);
	matInit(&a442, 4, 4);
	matInit(&a443, 4, 4);
	matInit(&a411, 4, 1);
	matInit(&a341, 3, 4);
	matInit(&a311, 3, 1);
	matInit(&a431, 4, 3);
	matInit(&a432, 4, 3);
	matInit(&a332, 3, 3);
	matInit(&a333, 3, 3);
	matInit(&a331, 3, 3);
	eyeMat(&a44I, 4);

	//Crear Wmat que es aux1

	setMatVal(&a441, 0, 1 , -gx);
	setMatVal(&a441, 0, 2 , -gy);
	setMatVal(&a441, 0, 3 , -gz);
	setMatVal(&a441, 1, 0 ,  gx);
	setMatVal(&a441, 1, 2 ,  gz);
	setMatVal(&a441, 1, 3 , -gy);
	setMatVal(&a441, 2, 0 ,  gy);
	setMatVal(&a441, 2, 1 , -gz);
	setMatVal(&a441, 2, 3 ,  gx);
	setMatVal(&a441, 3, 0 ,  gz);
	setMatVal(&a441, 3, 1 ,  gy);
	setMatVal(&a441, 3, 2 , -gx);

	matScale(&a441, &a441, 0.5*(t->Ts) );
	matAdd( &a441, &a441, &a44I );
	matMult( &a411, &a441, &(t->q) );
	matCopy(&(t->q), &a411);

	matTrans(&a442, &a441);
	matMult( &a443, &a441, &(t->P) );
	matMult( &(t->P), &a443, &a442 );
	matAdd( &(t->P), &(t->P), &(t->Q) );

	setMatVal(&a341, 0, 0, -2*getMatVal(&(t->q),2,0));
	setMatVal(&a341, 0, 1,  2*getMatVal(&(t->q),3,0));
	setMatVal(&a341, 0, 2, -2*getMatVal(&(t->q),0,0));
	setMatVal(&a341, 0, 3,  2*getMatVal(&(t->q),1,0));
	setMatVal(&a341, 1, 0,  2*getMatVal(&(t->q),1,0));
	setMatVal(&a341, 1, 1,  2*getMatVal(&(t->q),0,0));
	setMatVal(&a341, 1, 2,  2*getMatVal(&(t->q),3,0));
	setMatVal(&a341, 1, 3,  2*getMatVal(&(t->q),2,0));
	setMatVal(&a341, 2, 0,  2*getMatVal(&(t->q),0,0));
	setMatVal(&a341, 2, 1, -2*getMatVal(&(t->q),1,0));
	setMatVal(&a341, 2, 2, -2*getMatVal(&(t->q),2,0));
	setMatVal(&a341, 2, 3,  2*getMatVal(&(t->q),3,0));

	setMatVal(&a311, 0, 0,  2*( (getMatVal(&(t->q),1,0)*getMatVal(&(t->q),3,0)) - (getMatVal(&(t->q),0,0)*getMatVal(&(t->q),2,0)) ));	
	setMatVal(&a311, 1, 0,  2*( (getMatVal(&(t->q),0,0)*getMatVal(&(t->q),1,0)) + (getMatVal(&(t->q),2,0)*getMatVal(&(t->q),3,0)) ));
	setMatVal(&a311, 2, 0, getMatVal(&(t->q),0,0)*getMatVal(&(t->q),0,0) - getMatVal(&(t->q),1,0)*getMatVal(&(t->q),1,0) -getMatVal(&(t->q),2,0)*getMatVal(&(t->q),2,0) + getMatVal(&(t->q),3,0)*getMatVal(&(t->q),3,0));
	matScale(&a311, &a311, GRAVITY);						//h1


	matTrans(&a331, &(t->V1));								//V1'
	matMult(&a332, &(t->V1), &(t->R1));					//V1*R
	matMult(&a333, &a332, &a331);						//V1*R*V1'

	//printMat(&(t->P), "P");
	matTrans(&a431, &a341);									//H1'
	//printMat(&a431, "H'");
	matMult(&a432, &(t->P), &a431);							//P*H1'
	//printMat(&a432, "P*H'");
	matMult(&a332, &a341, &a432);							//H1*P*H'

	matAdd(&a332, &a332, &a333);							//H1*P*H1' + V1*R*V1'
	

	matInv(&a333, &a332);									//inv(H1*P*H1' + V1*R*V1)

	matMult(&a431, &a432, &a333);							//K1

	setMatVal(&a311, 0, 0, ax - getMatVal( &a311, 0, 0 ));
	setMatVal(&a311, 1, 0, ay - getMatVal( &a311, 1, 0 ));
	setMatVal(&a311, 2, 0, az - getMatVal( &a311, 2, 0 ));


	matMult(&a411, &a431, &a311 );
	setMatVal(&a411, 0, 3, 0);

	matAdd(&(t->q), &(t->q), &a411 );


	//`printMat(&(t->P), "P");
	matMult(&a441, &a431, &a341);
	matScale(&a441, &a441, -1.0);
	matAdd(&a442, &a44I, &a441);
	//printMat(&a44I, "I");

	//serialPrint(SER_DBG, "\n");
	//for(int i = 0 ; i < 4; i++){
	//	for(int j = 0 ; j < 4; j++) serialPrintf(SER_DBG, "%.1f\t", getMatVal(&a44I, i, j));
	//	serialPrint(SER_DBG, "\n");
	//}

	matMult(&a441, &a442 , &(t->P));
	matCopy(&(t->P),&a441);
	//printMat(&(t->P), "P");

	setMatVal(&a341, 0, 0,  2*getMatVal(&(t->q),3,0));
	setMatVal(&a341, 0, 1,  2*getMatVal(&(t->q),2,0));
	setMatVal(&a341, 0, 2,  2*getMatVal(&(t->q),1,0));
	setMatVal(&a341, 0, 3,  2*getMatVal(&(t->q),0,0));
	setMatVal(&a341, 1, 0,  2*getMatVal(&(t->q),0,0));
	setMatVal(&a341, 1, 1, -2*getMatVal(&(t->q),1,0));
	setMatVal(&a341, 1, 2, -2*getMatVal(&(t->q),2,0));
	setMatVal(&a341, 1, 3, -2*getMatVal(&(t->q),3,0));
	setMatVal(&a341, 2, 0, -2*getMatVal(&(t->q),1,0));
	setMatVal(&a341, 2, 1, -2*getMatVal(&(t->q),0,0));
	setMatVal(&a341, 2, 2,  2*getMatVal(&(t->q),3,0));
	setMatVal(&a341, 2, 3,  2*getMatVal(&(t->q),2,0));
	//printMat(&a341, "H2");


	setMatVal(&a311, 0, 0,  2*( getMatVal(&(t->q),1,0)*getMatVal(&(t->q),2,0) + getMatVal(&(t->q),3,0)*getMatVal(&(t->q),0,0)));
	setMatVal(&a311, 1, 0, getMatVal(&(t->q),0,0)*getMatVal(&(t->q),0,0) - getMatVal(&(t->q),1,0)*getMatVal(&(t->q),1,0) - getMatVal(&(t->q),2,0)*getMatVal(&(t->q),2,0) - getMatVal(&(t->q),3,0)*getMatVal(&(t->q),3,0));
	setMatVal(&a311, 2, 0,  2*( getMatVal(&(t->q),2,0)*getMatVal(&(t->q),3,0) - getMatVal(&(t->q),0,0)*getMatVal(&(t->q),1,0)));
	//printMat(&a311, "h2");


	matTrans(&a431, &a341);
	matMult(&a432, &(t->P), &a431 );
	matMult(&a331, &a341, &a432 );

	matTrans(&a332, &(t->V2));
	matMult(&a333, &(t->R2), &a332 );
	matMult(&a332, &(t->V2), &a333 );
	//printMat(&a332, "V*R2*V'");

	matAdd(&a331,&a331,&a332);

	matInv(&a333, &a331);

	matMult(&a431, &a432, &a333);


	setMatVal(&a311, 0, 0 , mx-getMatVal( &a311 , 0, 0));
	setMatVal(&a311, 1, 0 , my-getMatVal( &a311 , 1, 0));
	setMatVal(&a311, 2, 0 , mz-getMatVal( &a311, 2, 0));

	matMult(&a411, &a431, &a311);
	setMatVal(&a411,0, 1, 0);
	setMatVal(&a411,0, 2, 0);

	matAdd(&(t->q), &(t->q), &a411);

	matMult(&a441, &a431, &a341);
	matScale(&a441, &a441, -1.0);
	matAdd(&a442, &a44I, &a441);
	matMult( &a443, &a442, &(t->P)  );
	matCopy(&(t->P), &a443);

	 
	matDestruct(&a441);
	matDestruct(&a442);
	matDestruct(&a443);
	matDestruct(&a411);
	matDestruct(&a431);
	matDestruct(&a432);
	matDestruct(&a331);
	matDestruct(&a332);
	matDestruct(&a333);
	matDestruct(&a311);
	matDestruct(&a341);
	matDestruct(&a44I);
}


void getTkfEuler(tkf *t, float* euler){

	// roll (x-axis rotation)
	float sinr_cosp = 2 * (getMatVal(&(t->q),0,0) * getMatVal(&(t->q),1,0) + getMatVal(&(t->q),2,0) * getMatVal(&(t->q),3,0));
	float cosr_cosp = 1 - 2 * (getMatVal(&(t->q),1,0) * getMatVal(&(t->q),1,0) + getMatVal(&(t->q),2,0) * getMatVal(&(t->q),2,0));
	euler[0] = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = 2 * (getMatVal(&(t->q),0,0) * getMatVal(&(t->q),2,0) - getMatVal(&(t->q),3,0) * getMatVal(&(t->q),1,0));
	if (fabs(sinp) >= 1)
		euler[1] = copysign(pi / 2, sinp); // use 90 degrees if out of range
	else
		euler[1] = asin(sinp);

	// yaw (z-axis rotation)
	float siny_cosp = 2 * (getMatVal(&(t->q),0,0) * getMatVal(&(t->q),3,0) + getMatVal(&(t->q),1,0) * getMatVal(&(t->q),2,0));
	float cosy_cosp = 1 - 2 * (getMatVal(&(t->q),2,0) * getMatVal(&(t->q),2,0) + getMatVal(&(t->q),3,0) * getMatVal(&(t->q),3,0));
	euler[2] = atan2(siny_cosp, cosy_cosp);
}
