#ifndef SENSORS_TASKS_H
#define SENSORS_TASKS_H

#include "filter.h"
#include "MPU9250.h"
#include "ICM20948.h"
#include "registerMap.h"
#include "utils.h"
#include "BMP388.h"


#define MPU9250  1
#define ICM20948 2

#define IMU MPU9250


extern float    roll,       pitch,      yaw,
                raw_roll,   raw_pitch,  raw_yaw,
                ax,         ay,         az, 
                gx,         gy,         gz, 
                mx,         my,         mz, 
                x,          y,          z; 

extern mpu9250 myMPU;
extern icm20948 myICM;

extern bmp388 myBMP;
extern float altitude;

/**
    *@brief Initialize Sensors Tasks 
*/
void initSensorsTasks();

#endif