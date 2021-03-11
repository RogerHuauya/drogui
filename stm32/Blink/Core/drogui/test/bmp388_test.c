#define BMP388_TEST
#ifdef BMP388_TEST

#include "_main.h"
#include "BMP388.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "utils.h"

char buffer_bmp[50];

void _main(){

    initBmp388();
    while(1){
        
        int64_t temp = bmp388CompensateTemp(bmpReadTemperature());
        int64_t press = bmp388CompensatePress(bmpReadPressure());

        sprintf(buffer_bmp,"%lld \t %lld\n", temp, press);
        HAL_UART_Transmit(&huart2,buffer_bmp,strlen(buffer_bmp),1000);
        HAL_Delay(1000);
    }

}

#endif