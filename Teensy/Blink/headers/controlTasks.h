#ifndef CONTROL_TASKS_H
#define CONTROL_TASKS_H

#include "pwm.h"
#include "timer.h"
#include "control.h"
#include "sensorsTasks.h"
#include "utils.h"
#include "registerMap.h"

extern double M1,M2,M3,M4;


void initControlTasks();
void executeControlTasks();
void updatePID();

#endif