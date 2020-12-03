#ifndef KALMAN_H
#define KALMAN_H

#include "utils.h"
#include "matlib.h"

void kynematics(mat* p, mat* v, mat* u, mat* Rq,float Ts);


#endif