//#define DROGUI
#ifdef DROGUI
#include "_main.h"
#include "tim.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "debugTasks.h"
#include "task.h"
#include "serial.h"
char auxbuff[50];
void _main(){

    #if PORT == LED 
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    #endif
    HAL_Delay(1000);
    
    #if PORT == DEBUG
        serialPrint("Start System\n");
    #endif
    
    initDebug();
    initControlTasks();
    initSensorsTasks();

    //sprintf(auxbuff, "%d\n", __FPU_USED);
    //HAL_UART_Transmit(&huart2, (uint8_t*) auxbuff, strlen(auxbuff), 100);
    initRTOS();
}

#endif