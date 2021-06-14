#include "debugTasks.h"
#include "utils.h"
#include "serial.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "task.h"
#include "filter.h"
#include "macros.h"

void blinkTask(void *argument){
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}


void debugTask(void *argument){
    
    
    serialPrintf(SER_DBG, "%f %f %f %f\n",getReg(X_VAL), getReg(Y_VAL), getReg(ROLL_SCURVE), getReg(PITCH_SCURVE),getReg(RAW_YAW));
    //serialPrintf(SER_DBG, "%f %f %f\n", gx, gy, gz);
    //serialPrintf(SER_DBG, "%f %f %f\n", mx, my, mz);
	//serialPrintf(SER_DBG, "%f %f %f\n", ax, ay, az);
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
    addTask(&debugTask, 10000, 1);

    addTask(&blinkTask, 100000, 1);   
    
    addTask(&securityTask, 1000, 1);
}
