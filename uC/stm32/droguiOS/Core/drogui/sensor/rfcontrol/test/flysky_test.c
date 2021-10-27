#define FLYSKY_TEST
#ifdef FLYSKY_TEST
#include "_main.h"
#include "serial.h"
#include "flysky.h"

fsReceiver myFs;

void _main(){

	serialsBegin();
	changeBaudrate(SER_DBG, 1000000);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
	initFsReceiver(&myFs, &serial5);
	while(1){
		SENSOR_STATUS ret = readFsReceiver(&myFs);

		if(ret == OK){
			for(int i = 0; i < CHANNEL_NUM; i++) serialPrintf(SER_DBG, "Channel(%d) %d, ", i, myFs.channel_val[i]);
			serialPrint(SER_DBG, "\n");
		}
		else if(ret != NO_DATA )
			serialPrintf(SER_DBG, "Error = %d\n", ret);
		else
			serialPrint(SER_DBG, "No data\n");


		HAL_Delay(10);
	}

}

#endif
