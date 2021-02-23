#ifndef SENSORS_TASKS_H
#define SENSORS_TASKS_H

#include "filter.h"
#include "MPU9250.h"
#include "timer.h"
#include "registerMap.h"
#include "utils.h"
#include "kalman.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3XX.h>

extern float roll, pitch, yaw, ax, ay, az, gx, gy, gz, mx, my, mz, x, y, z;
extern mpu9250 myIMU;

void initSensorsTasks();
void executeSensorsTasks();

#endif