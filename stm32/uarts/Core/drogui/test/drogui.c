//#define DROGUI
#ifdef DROGUI
#include "_main.h"
#include "tim.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "debugTasks.h"
#include "task.h"
#include "serial.h"
#include"usart.h"
char auxbuff[50];
void _main(){
    /*
    
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);
    
    serialPrint(&serial3, "Start System\n");
    
    
    initDebug();
    initControlTasks();
    initSensorsTasks();

    //sprintf(auxbuff, "%d\n", __FPU_USED);
    //HAL_UART_Transmit(&huart2, (uint8_t*) auxbuff, strlen(auxbuff), 100);
    initRTOS();*/
    serialsBegin();
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    int i = 0;
    while(1){
        if(serialAvailable(&serial5)){
            serialPrintf(&serial5, "Recibo %c\n", serialRead(&serial5));
        }
        //serialPrint(&serial5, "hola\n");
        //HAL_UART_Transmit(&huart5, (uint8_t *) "Hola mundo\n", 12, 1000);
        //HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
       
    }
}

#endif