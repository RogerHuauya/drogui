//#define BMP388_TEST
#ifdef BMP388_TEST

#include "_main.h"
#include "BMP388.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "filter.h"
#include "utils.h"
#define N_BMP 25

char buffer_bmp[500];
bmp388 myBMP;

emaFilter ema_bmp;
mvAvgFilter mvAvg_bmp;

void _main(){
    
    HAL_UART_Transmit(&huart2,(uint8_t *)"hola mundo\n",12,1000);
    initBmp388(&myBMP, 10);  

    initMvAvgFilter(&mvAvg_bmp, N_BMP);
    initEmaFilter(&ema_bmp, 0.6, 0.4, 0.5);

    while(1){
        
        float bmp_alt;

        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);        
        bmp388ReadAltitude(&myBMP);
        
        bmp_alt = computeMvAvgFilter( &mvAvg_bmp, myBMP.altitude );
        bmp_alt = computeEmaFilter( &ema_bmp, bmp_alt );

        sprintf(buffer_bmp,"%f %f ;\n", 100*myBMP.altitude, 100*bmp_alt);
        HAL_UART_Transmit(&huart2,(uint8_t *)buffer_bmp,strlen(buffer_bmp),1000);
        HAL_Delay(5);

    }

}

#endif