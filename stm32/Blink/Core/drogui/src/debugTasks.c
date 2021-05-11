#include "debugTasks.h"
#include "utils.h"
#include "serial.h"
#include "sensorsTasks.h"
#include "controlTasks.h"
#include "task.h"
#include "filter.h"

#if PORT == LED
    void blinkTask(void *argument){
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }
#endif

#if PORT == DEBUG
    void debugTask(void *argument){
        
        /*if(serialAvailable()){
            char c = serialRead();
            //serialPrintf("%x ",(uint8_t) c);
            if( c == 'a' ) setReg(START_GPS,1);
            else setReg(START_GPS,0);
        }*/
        
        serialPrintf("%f\t%f\t%f;\n", gx, gy, gz);
        //serialPrint("I am debugging\n");

    }
#endif

void securityTask(){
    if(getReg(Z_REF) == 0 || (fabs(angle_dif(roll_ref, roll))> pi/9) || (fabs(angle_dif(pitch_ref, pitch))> pi/9)){
        updatePID();
        
        #if IMU == ICM20948
            if(getReg(CAL_GYR_TRG) == 1) calibrateIcmGyro(&myICM),  setReg(CAL_GYR_TRG, 0);
            if(getReg(CAL_ACC_TRG) == 1) calibrateIcmAccel(&myICM), setReg(CAL_ACC_TRG, 0);
        #elif IMU == MPU9250
            if(getReg(CAL_GYR_TRG) == 1) calibrateMpuGyro (&myMPU), setReg(CAL_GYR_TRG, 0);
            if(getReg(CAL_ACC_TRG) == 1) calibrateMpuAccel(&myMPU), setReg(CAL_ACC_TRG, 0);
        #endif

        if(getReg(CAL_MAG_TRG) == 1) calibrateMpuMag(&myMPU),   setReg(CAL_MAG_TRG, 0);
        updateBmp388Offset(&myBMP);
        security = true;
        calib_status = updateMpuCalibOffset(&myMPU) | updateIcmCalibOffset(&myICM);
    }
    else security = false;
}

void initDebug(){
    #if PORT == DEBUG   
        //addTask(&debugTask, 10000, 1);
    #elif PORT == LED
        addTask(&blinkTask, 100000, 1);   
    #endif
    addTask(&securityTask, 1000, 1);
}