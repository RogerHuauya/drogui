#ifndef TASK_H
#define TASK_H
#include "tim.h"

#define TIME ((uint32_t) __HAL_TIM_GET_COUNTER(&htim5))
/**
*   @brief Schedule a task with a certain priority and period
*   @param f Task to be done (void*)
*   @param preiod Period(uint32_t)
*   @param priority Priority (int)
*/
void addTask(void (*f) (), uint32_t period, int priority);
/**
*   @brief Initialize the Real Time System
*
*/
void initRTOS();

#endif