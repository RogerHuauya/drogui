#include "main.h"
#include <string.h>
#include "mylib.h"
#include "lib_test.h"

void _main(){
    char message[50] = "Hello drogui!!\r\n";
    while (1){

        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        HAL_Delay(suma(250,250) + resta(1000,500));
        HAL_UART_Transmit(&huart1, (uint8_t*) message, strlen(message), 1000);
    }

}