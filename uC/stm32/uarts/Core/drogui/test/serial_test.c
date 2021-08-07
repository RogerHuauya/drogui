//#define SERIAL_TEST
#ifdef SERIAL_TEST

#include "_main.h"
#include "serial.h"


void _main(){

	serialsBegin();
	HAL_Delay(1000);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	changeBaudrate(&serial4, 115200);
	serialPrint(&serial4, "test\n");
	while(1){
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		while(serialAvailable(&serial4)){
			serialWrite(&serial4, serialRead(&serial4));
		}

		HAL_Delay(100);
	}

}


#endif
