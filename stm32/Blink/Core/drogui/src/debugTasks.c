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
    
    sprintf(buff, "%f\t%f\t%f\t%f\t%ld;\n", myIMU.gx/5, gx, myIMU.ax,ax,TIME);
    //sprintf(buff, "%lu;\n",TIME);
    HAL_UART_Transmit(&huart2, (uint8_t*) buff, strlen(buff), 100);
}

void initDebug(){
    
    //#if(__FPU_PRESENT == 1) && (__FPU_USED == 1)
    addTask(&debugTask, 1000, 1);
    addTask(&blinkTask, 1000000, 1);
    //#endif

}