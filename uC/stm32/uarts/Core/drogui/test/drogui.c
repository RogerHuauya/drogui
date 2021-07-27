#define DROGUI
#ifdef DROGUI
#include "_main.h"
#include "tim.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "debugTasks.h"
#include "task.h"
#include "serial.h"
#include "usart.h"
#include "macros.h"
char auxbuff[50];
void _main(){
    serialsBegin();
    changeBaudrate(SER_DBG, 2000000);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);

    serialPrint(SER_DBG, "Start System\n");

    initDebug();
    initControlTasks();
    initSensorsTasks();

    initRTOS();
}

#endif
