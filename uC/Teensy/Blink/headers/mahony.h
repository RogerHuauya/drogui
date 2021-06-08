#ifndef MAHONY_H
#define MAHONY_h
#include <stdint.h>
#include <math.h>
#include "matlib.h"


void mahonyUpdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void mahonyUpdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
void getMahonyQuaternion(float* quat);
void getMahonyEuler(float* euler);

#endif
