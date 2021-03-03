#ifndef TASK_H
#define TASK_H
#include "tim.h"

#define TIME ((uint32_t) __HAL_TIM_GET_COUNTER(&htim5))

void addTask(void (*f) (), uint32_t period, int priority);
void initRTOS();

#endif