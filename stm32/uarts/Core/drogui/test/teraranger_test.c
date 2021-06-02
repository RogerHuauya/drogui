#define TERARANGER
#ifdef TERARANGER
#include "_main.h"
#include "serial.h"
#include "teraRanger.h"

tRanger myTerat;

void _main(){

	serialsBegin();
	HAL_Delay(1000);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	initTeraRanger(&myTerat, &serial2);
	while(1){
		SENSOR_STATUS ret = readTeraRange(&myTerat);

		if(ret == OK)
			serialPrintf(&serial3, "Dis = %d\n", myTerat.distance);
		else if(ret != NO_DATA )
			serialPrintf(&serial3, "Error = %d\n", ret);
		else
			serialPrint(&serial3, "No data\n");


		HAL_Delay(10);
	}

}

#endif
