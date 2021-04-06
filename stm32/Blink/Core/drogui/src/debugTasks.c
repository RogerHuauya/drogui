#include "debugTasks.h"
#include "utils.h"
#include "serial.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "task.h"
#include <stdio.h>
#include <string.h>
#include "filter.h"

#if PORT == LED
    void blinkTask(void *argument){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }
#endif

#if PORT == DEBUG
char debug_buffer[500] = "hola\n";
void debugTask(void *argument){
    
    //sprintf(buff, "%f\n", z);
    int ind = 0;
    while(serialAvailable()){
        debug_buffer[ind++] = serialRead();
    }
    debug_buffer[ind] = '\0';
    serialPrint(debug_buffer);
}
#endif

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
    #if PORT == DEBUG   
        addTask(&debugTask, 10000, 1);
    #elif PORT == LED
        addTask(&blinkTask, 100000, 1);   
    #endif
    //addTask(&securityTask, 1000, 1);
}