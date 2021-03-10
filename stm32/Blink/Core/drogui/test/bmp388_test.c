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

    float altitude = 0;
    int64_t zerolevel = bmp388CompensatePress(bmpReadPressure());    

    


    while(1){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

        
        int64_t temp = bmp388CompensateTemp(bmpReadTemperature());
        int64_t press = bmp388CompensatePress(bmpReadPressure());

        altitude = 44330.0 * (1.0 -  pow( (1.0*press / zerolevel),0.1903)); 
        
        sprintf(buffer_bmp,"%ld ;\n",(int32_t)press);
        HAL_UART_Transmit(&huart2,(uint8_t *)buffer_bmp,strlen(buffer_bmp),1000);
        HAL_Delay(5);




    }

}

#endif