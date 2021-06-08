//#define PRINCIPAL_TEST
#ifdef PRINCIPAL_TEST

#include "_main.h"
#include <string.h>

void _main(){
    char message[50] = "Hello drogui!!\r\n";
    while (1){

        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        HAL_Delay(250);
        HAL_UART_Transmit(&huart1, (uint8_t*) message, strlen(message), 1000);
    }

}
#endif