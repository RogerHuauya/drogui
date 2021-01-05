#ifndef KALMAN_H
#define KALMAN_H

#include <Arduino.h>
#include "utils.h"
#include "matlib.h"

void initMatGlobal();
void kynematics();
void getMatFm();
void getMatGm();
void UpdatePm();
void getKalmanGain();
void UpdatePmCovGPS();
void getBias();
void kalmanUpdateIMU(float ax, float ay, float az, float roll, float pitch,float yaw);
void kalmanUpdateGPS(float x_gps, float y_gps, float z_gps);
void setKalmanTsImu(float ts);
void getPosition(float *x, float *y, float *z);
void setKalmanTsGps(float ts);
void clearKalman();


#endif
