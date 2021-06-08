#ifndef KALMAN_H
#define KALMAN_H

#include <Arduino.h>
#include "matlib.h"

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