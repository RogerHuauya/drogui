#ifndef SENSORS_TASKS_H
#define SENSORS_TASKS_H

#include "filter.h"
#include "MPU9250.h"
#include "ICM20948.h"
#include "registerMap.h"
#include "utils.h"
#include "BMP388.h"


extern float	roll,       pitch,      yaw,
				raw_roll,   raw_pitch,  raw_yaw,
				ax,         ay,         az, 
				gx,         gy,         gz, 
				mx,         my,         mz, 
				x,          y,          z, 
				x_gps,		y_gps,
				xp,         yp,
				z_of,       z_tera; 


extern imu myIMU;


extern bmp388 myBMP; 
extern float altitude;

/**
    *@brief Initialize Sensors Tasks 
*/
void initSensorsTasks();

#endif
