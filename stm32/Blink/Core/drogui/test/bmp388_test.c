#define BMP388_TEST
#ifdef BMP388_TEST

#include "_main.h"
#include "BMP388.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "utils.h"

char buffer_bmp[500];
bmp388 myBMP;

void _main(){
    
    HAL_UART_Transmit(&huart2,(uint8_t *)"hola mundo\n",12,1000);
    initBmp388(&myBMP, 10);


    float altitude = 0;    

    while(1){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

        bmp388ReadAltitude(&myBMP);

        sprintf(buffer_bmp,"%f ;\n", myBMP.altitude);
        HAL_UART_Transmit(&huart2,(uint8_t *)buffer_bmp,strlen(buffer_bmp),1000);
        
        HAL_Delay(5);




    }

}

#endif