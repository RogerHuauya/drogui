#include "debugTasks.h"
#include "utils.h"
#include "usart.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include "filter.h"

char buff[50] = "hola\n";
void debugTask(void *argument){
    
    sprintf(buff, "%f\n", z);
    HAL_UART_Transmit(&huart2, (uint8_t*) buff, strlen(buff), 100);
}
void securityTask(){
    if(getReg(Z_REF) == 0 || (fabs(angle_dif(roll_ref, roll))> pi/9) || (fabs(angle_dif(pitch_ref, pitch))> pi/9)){
        updatePID();
        if(getReg(CAL_GYR_TRG) == 1) calibrateGyro(&myIMU), setReg(CAL_GYR_TRG, 0);
        if(getReg(CAL_ACC_TRG) == 1) calibrateAccel(&myIMU), setReg(CAL_ACC_TRG, 0);
        if(getReg(CAL_MAG_TRG) == 1) calibrateMag(&myIMU), setReg(CAL_MAG_TRG, 0);
        updateBmp388Offset(&myBMP);
        security = true;
        calib_status = updateCalibOffset(&myIMU);
    }
    else security = false;
}

void initDebug(){
    
    //addTask(&debugTask, 10000, 1);   
    addTask(&securityTask, 1000, 1);
}