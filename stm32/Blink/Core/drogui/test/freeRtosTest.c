//#define FREE_RTOS_TEST
#ifdef FREE_RTOS_TEST

#include "_main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "_freertos.h"
#include "sensorsTasks.h"


osThreadId_t debugHandle;
const osThreadAttr_t debugAttributes = {    .name = "debugTask", \
                                            .stack_size = 512 * 4, \
                                            .priority = (osPriority_t) osPriorityNormal};


osThreadId_t blinkHandle;
const osThreadAttr_t blinkAttributes = {    .name = "blinkTask",\
                                            .stack_size = 128 * 4,\
                                            .priority = (osPriority_t) osPriorityNormal};

void blinkTask(void *argument){

    for(;;){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        osDelay(1000);
    }
}

char buff[50] = "hola\n";
void debugTask(void *argument){
    
    for(;;){
        sprintf(buff, "%f\t%f\t%f;\n", myMPU.gx/5, gx, gy);
        HAL_UART_Transmit(&huart2, (uint8_t*) buff, strlen(buff), 100);
        osDelay(1);
    }
}

void initSystem(){
    blinkHandle = osThreadNew(blinkTask, NULL, &blinkAttributes);
    debugHandle = osThreadNew(debugTask, NULL, &debugAttributes);
}

#endif