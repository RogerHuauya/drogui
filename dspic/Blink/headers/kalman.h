#ifndef KALMAN_H
#define KALMAN_H

#include "utils.h"
#include "matlib.h"
#include "serial.h"
#include "i2c.h"
#include "registerMap.h"

extern mat p, v, Rq, s;
void initMatGlobal();
void kynematics();
void getMatFm();
void getMatGm();
void UpdatePm();
void getKalmanGain();
void UpdatePmCovGPS();
void getBias();
void kalmanUpdate();
#endif