//#define PWM_TEST
#ifdef PWM_TEST

#include "_main.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "tim.h"


osThreadId_t debugHandle;
const osThreadAttr_t debugAttributes = {    .name = "debugTask", \
                                            .stack_size = 512 * 4, \
                                            .priority = (osPriority_t) osPriorityNormal};


osThreadId_t blinkHandle;
const osThreadAttr_t blinkAttributes = {    .name = "blinkTask",\
                                            .stack_size = 128 * 4,\
                                            .priority = (osPriority_t) osPriorityNormal};

osThreadId_t pwmHandle;
const osThreadAttr_t pwmAttributes = {    .name = "pwmTask",\
                                            .stack_size = 128 * 4,\
                                            .priority = (osPriority_t) osPriorityNormal};


void pwmTask(void *argument){

    for(;;){
        htim3.Instance->CCR1 = 25000;
        htim3.Instance->CCR2 = 12500;
        htim4.Instance->CCR3 = 20000;
        htim4.Instance->CCR4 = 10000;
        osDelay(1000);
    }
}


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
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

    blinkHandle = osThreadNew(blinkTask, NULL, &blinkAttributes);
    //debugHandle = osThreadNew(debugTask, NULL, &debugAttributes);
    pwmHandle = osThreadNew(pwmTask, NULL, &pwmAttributes);

}

#endif
