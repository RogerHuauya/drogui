//#define SERIAL_TEST
#ifdef SERIAL_TEST

#include "_main.h"
#include "serial.h"


void _main(){

	serialsBegin();
	HAL_Delay(1000);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	changeBaudrate(&serial3, 115200);
	changeBaudrate(&serial2, 115200);
	serialPrint(&serial3, "test\n");
	while(1){
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		while(serialAvailable(&serial2)){
			serialPrintf(&serial3, "%x\n", serialRead(&serial2));

		}

		HAL_Delay(100);
	}

}


#endif
