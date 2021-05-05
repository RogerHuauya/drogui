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
char buff_mt[50];

float ax_filt;

void _main(){
    initMpu(&myImu);
    //calibrateGyro(&myImu); 
    //calibrateAccel(&myImu);
    calibrateMag(&myImu);
    initFilter(&my_filter, 6, k_1_10, v_1_10);

    while(1){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        readMag(&myImu);
        //ax_filt = computeFilter(&my_filter, myImu.gx);
        sprintf(buff_mt, "Hola %f \t %f \t %f;\n", myImu.mx, myImu.my, myImu.mz);
        HAL_UART_Transmit(&huart2, (uint8_t *)buff_mt, strlen(buff_mt), 100); 
        HAL_Delay(100);
    }
}



#endif