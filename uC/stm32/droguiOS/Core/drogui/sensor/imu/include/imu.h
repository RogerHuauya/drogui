#ifndef IMU_H
#define IMU_H

#include <macros.h>

#ifdef ICM20948
	#include <ICM20948.h>
#elif defined MPU9250
	#include <MPU9250.h>
#endif

#endif
