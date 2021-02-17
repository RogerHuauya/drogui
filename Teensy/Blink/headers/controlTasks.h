#ifndef CONTROL_TASKS_H
#define CONTROL_TASKS_H

#include "pwm.h"
#include "timer.h"
#include "control.h"
#include "sensorsTasks.h"
#include "utils.h"
#include "registerMap.h"



void initControlTasks();
void executeControlTasks();
void updatePID();

#endif