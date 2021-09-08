#ifndef TKF_H
#define TKF_H

#include "utils.h"
#include "matlib.h"


typedef struct _tkf{
	float Ts;
	mat q;					// quaternion of sensor frame relative to auxiliary frame
	mat V1, V2, Q, P, R1, R2;
} tkf;

void initTkf(tkf *tkf, float Ts);

void getTkfEuler(tkf *t, float* euler);
void tkfUpdate(tkf *t, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
#endif
