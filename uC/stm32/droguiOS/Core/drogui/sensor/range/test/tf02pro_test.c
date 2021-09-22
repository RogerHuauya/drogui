#define TF02PRO_TEST
#ifdef TF02PRO_TEST
#include "_main.h"
#include "serial.h"
#include "rangeFinder.h"

 rangeFinder myTf02;

void _main(){

	serialsBegin();
	changeBaudrate(SER_DBG, 1000000);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	initRangeFinder(&myTf02, &serial2);
	while(1){
		SENSOR_STATUS ret = readRangeFinder(&myTf02);

		if(ret == OK)
			serialPrintf(&serial3, "Dis = %d ", myTf02.distance), serialPrintf(&serial3, "strength = %d\n", myTf02.strength);
		else if(ret != NO_DATA )
			serialPrintf(&serial3, "Error = %d\n", ret);
		else
			serialPrint(&serial3, "No data\n");

		HAL_Delay(10);

	}

}

#endif
