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

    HAL_TIM_Base_Start(&htim5);
    

    initDebug();
    initSensorsTasks();
    initRTOS();

}

#endif