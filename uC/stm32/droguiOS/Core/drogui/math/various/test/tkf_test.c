//#define TKF_TEST
#ifdef TKF_TEST

//#include "sensorsTasks.h"
#include "_main.h"
#include "macros.h"
#include "serial.h"
#include "tkf.h"
#include "task.h"

tkf myTKF;
float myArr[3];
void tkfTask(){
	tkfUpdate(&myTKF, 1, 1, 0, 0,0 ,9.81, 1, 0.5, 0.5);
	getTkfEuler(&myTKF,myArr);
	serialPrintf(SER_DBG, "%f\t%f\t%f\n", myArr[0], myArr[1], myArr[2]);
}

void _main(){

	serialsBegin();
	changeBaudrate(SER_DBG, 250000);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);

	serialPrint(SER_DBG, "Start System\n");

	//initSensorsTasks();

	initTkf(&myTKF,1/500.0);
	addTask(tkfTask, 100000, 1);

	initRTOS();
}

#endif
