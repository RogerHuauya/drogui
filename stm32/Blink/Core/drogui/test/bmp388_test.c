#define BMP388_TEST
#ifdef BMP388_TEST

#include "_main.h"
#include "BMP388.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "utils.h"

char buffer_bmp[500];

void _main(){
    
    HAL_UART_Transmit(&huart2,(uint8_t *)"hola mundo\n",12,1000);
    initBmp388();
    
    while(1){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        
        int64_t temp = bmp388CompensateTemp(bmpReadTemperature());
        int64_t press = bmp388CompensatePress(bmpReadPressure());

        sprintf(buffer_bmp,"%ld \t %ld \n", (int32_t)temp, (int32_t)press);
        HAL_UART_Transmit(&huart2,(uint8_t *)buffer_bmp,strlen(buffer_bmp),1000);
        HAL_Delay(50);
    }

}

#endif