#define DROGUI
#ifdef DROGUI
#include "_main.h"
#include "tim.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "debugTasks.h"
#include "task.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

void _main(){

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    HAL_TIM_Base_Start(&htim5);
    
    htim3.Instance->CCR1 = 12500;
    htim3.Instance->CCR2 = 12500;
    htim4.Instance->CCR3 = 12500;
    htim4.Instance->CCR4 = 12500;

    HAL_Delay(5000);

    htim3.Instance->CCR1 = 14000;
    htim3.Instance->CCR2 = 14000;
    htim4.Instance->CCR3 = 15000;
    htim4.Instance->CCR4 = 25000;

    initDebug();
    initSensorsTasks();
    initRTOS();

}

#endif