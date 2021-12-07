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
	//serialPrintf(SER_DBG, "%f,%f,%f,", ax, ay, az);
	//serialPrintf(SER_DBG, "%f,%f,%f,", gx, gy, gz);
	//serialPrintf(SER_DBG, "%f,", gz);
	//serialPrintf(SER_DBG, "%f,%f,%f", mx, my, mz);
	//serialPrintf(SER_DBG, "%f,%f,%f ", raw_roll*180/pi, raw_pitch*180/pi, raw_yaw*180/pi);
	//serialPrintf(SER_DBG, "%f,%f,%f,%f", getMatVal(&(myTKF.q), 0, 0), getMatVal(&(myTKF.q), 1, 0),getMatVal(&(myTKF.q), 2, 0),getMatVal(&(myTKF.q), 3, 0));
	//serialPrintf(SER_DBG, "%f,%f,%f,%f", myRPY.q0, myRPY.q1, myRPY.q2, myRPY.q3);
	//serialPrintf(SER_DBG, "%f ", raw_yaw*180/pi);
	//serialPrintf(SER_DBG, "%f ", tkf_roll*180/pi);//, tkf_pitch*180/pi, tkf_yaw*180/pi);
	//serialPrintf(SER_DBG, "%f ", mah_roll*180/pi);//, mah_pitch*180/pi, mah_yaw*180/pi);
	//serialPrintf(SER_DBG, "%f %f %f %f", vx_gps, vy_gps, vx, vy);
	//serialPrintf(SER_DBG, "%f ", z*100);
	//serialPrintf(SER_DBG, "Fsval %.3f,%.3f,%.3f,%.3f", myFS.channel_offset[0], myFS.channel_offset[1],myFS.channel_offset[2],myFS.channel_offset[3]);	
	//serialPrintf(SER_DBG, "%.3f,%.3f,%.3f,%.3f, %d", roll_fs, pitch_fs, yaw_fs, h_fs, state);	
	//serialPrintf(SER_DBG, "%.3f, %.3f, %.3f, %.3f", );
	//serialPrintf(SER_DBG, "%d, %.3f, %.3f, %.3f ", state, roll_ref, pitch_ref, yaw_ref);
	/*
	if(serialAvailable(SER_DBG)){
		char command = serialRead(SER_DBG);
		serialPrintf(SER_DBG, "%c ", command);
		switch (command){
			case 'A': setReg(ARM, 1.0);
			case 'S': setReg(START, 1.0);
			case 'D': setReg(STOP, 1.0);
			
		}
	}
	*/
	serialPrintf(SER_DBG, "%d %.3f %.3f %.3f",state, getReg(YAW_U), wyaw_ref, gz);
	//serialPrintf(SER_DBG, "%f ", z_ref);
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

		if(getReg(CAL_GYR_TRG) == 1) calibrateGyro(&myIMU), myRange.offset = myRange.distance;  setReg(CAL_GYR_TRG, 0);
		if(getReg(CAL_ACC_TRG) == 1) calibrateAccel(&myIMU), setReg(CAL_ACC_TRG, 0);
		if(getReg(CAL_MAG_TRG) == 1) calibrateMag(&myIMU),   setReg(CAL_MAG_TRG, 0);
		if(getReg(CAL_FS_TRG) == 1)  calibrateFsReceiver(&myFS), setReg(CAL_FS_TRG, 0);
		updateBmp388Offset(&myBMP);
		calib_status = updateCalibOffset(&myIMU);
	}
}

void initDebug(){
	state = SEC_STOP;
	addTask(&debugTask, 10000, 1);
	addTask(&blinkTask, 100000, 1);
	addTask(&securityTask, 1000, 1);
}
