#define MPU9250_TEST
#ifdef MPU9250_TEST

#include "_main.h"
#include "MPU9250.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "arm_math.h"

arm_iir_lattice_instance_f32 my_filter;

mpu9250 myImu;
char buff[50] = "Hola\n";

void _main(){
    initMpu(&myImu);
    calibrateGyro(&myImu); 
    calibrateAccel(&myImu);
    //arm_iir_lattice_init_f32(&my_filter, )
    while(1){
        
        readAcc(&myImu);
        sprintf(buff, "%f \t %f \t %f \n", myImu.ax, myImu.ay, myImu.az);
        HAL_UART_Transmit(&huart1, (uint8_t *)buff, strlen(buff), 100);
        HAL_Delay(100);
    }
}



#endif