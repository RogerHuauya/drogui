//#define ZOBSTACLE_TEST 
#ifdef ZOBSTACLE_TEST

#include "_main.h"
#include "tim.h"
#include "task.h"
#include "filter.h"
#include "registerMap.h"
#include "serial.h"
#include "usart.h"
#include "macros.h"
#include "utils.h"
#include "opticalFlow.h"
#include "teraranger.h"
#include "BMP388.h"

optFlow myOFL;
tRanger myTeraran;
bmp388 myBMP388;

emaFilter ema_bmp388;
mvAvgFilter mvAvg_bmp388;
filter filter_press;

float      z_optical,    z_teraranger,  
	       press_ant,       press_bmp, 
	          z_prev,           z_val,
            z_offset;

bool obstacle = false;

void printTask(void *argument){

	serialPrintf(SER_DBG, "%f,%f,%f", z_val + z_offset, z_val, z_offset);
	serialPrintf(SER_DBG, "\n");
}
void pressTask(){
	bmp388ReadAltitude(&myBMP388);
	//bmp388ReadPressure(&myBMP388);
	press_bmp = computeEmaFilter( &ema_bmp388, myBMP388.altitude);
	press_bmp = computeFilter( &filter_press, press_bmp );
}

void opticalTask(){

	OPT_VAR var;
	SENSOR_STATUS ret = readFlowRange(&myOFL, &var);

	if(ret == OK){
		if(myOFL.dis != -1) z_optical= myOFL.dis*0.001;
		setReg(Z_RNG, z_optical);
	}
	else if(ret == CRASHED){
		serialPrint(SER_DBG, "OPT Crashed\n");
		if(state == ARM_MOTORS || state == CONTROL_LOOP)
			state = DESCEND;
	}
}

void terarangerTask(){

	SENSOR_STATUS ret = readTeraRange(&myTeraran);

	if(ret == OK)
		z_teraranger = myTeraran.distance/1000.0;
	else if(ret == CRASHED){
		serialPrint(SER_DBG, "Tera Crashed\n");
		if(state == ARM_MOTORS || state == CONTROL_LOOP)
			state = DESCEND;
	}
}
void obstacleTask(){

	z_val = z_optical;
	if(z_teraranger >= 0.5 && z_teraranger <= 50)
		z_val = z_teraranger;
	
	if( fabs( z_prev - z_val) > 0.5 /* && fabs(press_ant-press_bmp) > 10.0*/ ){
 		obstacle = true;
	  	z_offset += (z_prev - z_val);
	}
	
	//if(obstacle) z_val += z_offset;

	z_prev = z_val;
	press_ant = press_bmp;

	setReg(Z_VAL,z_val);
}
void _main(){

    serialsBegin();
    changeBaudrate(SER_DBG, 2000000);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    HAL_Delay(1000);

	initOptFlow(&myOFL, SER_OPT);
	initTeraRanger(&myTeraran, SER_TER);

	initBmp388(&myBMP388, 10);
	initMvAvgFilter(&mvAvg_bmp388, 25);

	initEmaFilter(&ema_bmp388, 0.9, 0.1, 0.8);
	initFilter(&filter_press, 4, k_1_20, v_1_20);

	z_prev = press_ant = 0;
	z_offset = 0;

	addTask(&printTask, 10000, 1);
	addTask(&pressTask,10000,2);
	addTask(&opticalTask, 10000, 1);
	addTask(&terarangerTask, 10000, 1);
	addTask(&obstacleTask, 10000, 2);
	
    initRTOS();
}

#endif
