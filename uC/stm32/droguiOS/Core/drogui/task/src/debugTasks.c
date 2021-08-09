#include "debugTasks.h"
#include "utils.h"
#include "serial.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "task.h"
#include "filter.h"
#include "macros.h"

void blinkTask(void *argument){
	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}


void debugTask(void *argument){


	//serialPrintf(SER_DBG, "%f %f %f %f\n", xp_ref, xp,xp_ref -xp, pitch_ref);
	//serialPrintf(SER_DBG, "%f %f %f ", ax, ay, az);
	serialPrintf(SER_DBG, "%f %f %f ", gx, gy, gz);
	//serialPrintf(SER_DBG, "%f %f %f ", mx, my, mz);
	//serialPrintf(SER_DBG, "%f %f %f ", raw_roll*180/pi, raw_pitch*180/pi, raw_yaw*180/pi);
	//serialPrintf(SER_DBG, "%f %f %f %f", vx_gps, vy_gps, vx, vy);
	//serialPrintf(SER_DBG, "%f ", z*100);
	serialPrintf(SER_DBG, "\n");
}

void securityTask(){
	if(getReg(DESC) > 0) state = DESCEND, setReg(DESC, 0);
	if(getReg(STOP) > 0) state = SEC_STOP, setReg(STOP, 0);
	if(getReg(ARM) > 0){
		if(state == SEC_STOP) state = ARM_MOTORS;
		setReg(ARM, 0);
	}
	if(getReg(START) > 0){
		if(state == ARM_MOTORS) state = CONTROL_LOOP;
		setReg(START, 0);
	}


	if((fabs(angle_dif(roll_ref, roll))> pi/9) || (fabs(angle_dif(pitch_ref, pitch))> pi/9)) state = SEC_STOP;


	if(state == SEC_STOP || state == DESCEND){
		updatePID();

		if(getReg(CAL_GYR_TRG) == 1) calibrateGyro(&myIMU),  setReg(CAL_GYR_TRG, 0);
		if(getReg(CAL_ACC_TRG) == 1) calibrateAccel(&myIMU), setReg(CAL_ACC_TRG, 0);
		if(getReg(CAL_MAG_TRG) == 1) calibrateMag(&myIMU),   setReg(CAL_MAG_TRG, 0);
		updateBmp388Offset(&myBMP);
		calib_status = updateCalibOffset(&myIMU);
	}
}

void initDebug(){
	state = SEC_STOP;
	//addTask(&debugTask, 10000, 1);
	addTask(&blinkTask, 100000, 1);
	addTask(&securityTask, 1000, 1);
}
