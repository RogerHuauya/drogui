//#define MPU9250_TEST
#ifdef MPU9250_TEST

#include "_main.h"
#include "MPU9250.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "filter.h"
#include "utils.h"

filter my_filter;
mpu9250 myImu;
char buff[50];

float ax_filt;

void _main(){
    initMpu(&myImu);
    calibrateGyro(&myImu); 
    //calibrateAccel(&myImu);
    initFilter(&my_filter, sz_1_10, k_1_10, v_1_10);
    
    while(1){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        readGyro(&myImu);
        ax_filt = computeFilter(&my_filter, myImu.gx);
        sprintf(buff, "%f \t %f;\n", myImu.gx, ax_filt);
        
        HAL_UART_Transmit(&huart2, (uint8_t *)buff, strlen(buff), 100);
        HAL_Delay(1);
    }
}



#endif