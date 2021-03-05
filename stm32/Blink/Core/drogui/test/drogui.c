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
char auxbuff[50];
void _main(){

    HAL_TIM_Base_Start(&htim5);
    
    
    HAL_UART_Transmit(&huart2, (uint8_t*) "start system\n", 14, 100);
    initDebug();
    //initSensorsTasks();
    initControlTasks();
    sprintf(auxbuff, "%f\n", getReg(N_FILTER));
    HAL_UART_Transmit(&huart2, (uint8_t*) auxbuff, strlen(auxbuff), 100);
    initRTOS();

}

#endif