#include "debugTasks.h"
#include "utils.h"
#include "usart.h"
#include "sensorsTasks.h"
#include "task.h"
#include <stdio.h>
#include <string.h>


void blinkTask(void *argument){

    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

char buff[50] = "hola\n";
void debugTask(void *argument){
    
    sprintf(buff, "%f\t%f\t%lu ;\n", myIMU.gx/5, gx, TIME);
    HAL_UART_Transmit(&huart2, (uint8_t*) buff, strlen(buff), 100);
}

void initDebug(){
    //addTask(&debugTask, 1000, 1);
    addTask(&blinkTask, 1000000, 1);
}