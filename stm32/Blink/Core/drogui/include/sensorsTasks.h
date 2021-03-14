#ifndef SENSORS_TASKS_H
#define SENSORS_TASKS_H

#include "filter.h"
#include "MPU9250.h"
#include "registerMap.h"
#include "utils.h"
#include "BMP388.h"

extern float roll, pitch, yaw, ax, ay, az, gx, gy, gz, mx, my, mz, x, y, z;
extern mpu9250 myIMU;

extern bmp388 myBMP;
extern float altitude;

/**
    *@brief Initialize Sensors Tasks 
*/
void initSensorsTasks();

#endif