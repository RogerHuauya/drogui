//#define ICM_20948_TEST
#ifdef ICM_20948_TEST
#include "ICM20948.h"
#include "controlTasks.h"
#include "sensorsTasks.h"
#include "task.h"
#include "debugTasks.h"
#include "MPU9250.h"
#include "serial.h"
#include "_main.h"

/*icm20948 myicm;
mpu9250 mympu;*/

void icmTask(){

    readIcmRawGyro(&myICM);
    readMpuRawGyro(&myMPU);

    float val1 = myICM.raw_gx/65.5 + myICM.off_gx;
    float val2 = myMPU.raw_gx/65.5 + myMPU.off_gx;

    serialPrintf("%f\n", val1);


    //serialPrintf("%f\t", val);
    /*val = computeFilter(&(myICM.fGyroX.first), val);
    serialPrintf("%f\t", val);
    ///val = computeFilter(&(myICM.fGyroX.second), val);
    //serialPrintf("%f\t", val);
    val = computeDNotch(&(myICM.fGyroX.third), val);
    serialPrintf("%f\n", val);*/
    //val = computeDNotch(&(myICM.fGyroX.fourth), val);
    //serialPrintf("%f\n", val);
    // serialPrint("loop\n");
    

}


void _main(){
    
    HAL_Delay(1000);
    serialPrint("Starting...\n");
    initIcm(&myICM);
    initMpu(&myMPU);
    serialPrint("ICM started...!\n");
    calibrateIcmGyro(&myICM);
    calibrateMpuGyro(&myMPU);
    //while(1){
        
    initDebug();
    initControlTasks();
    addTask(&icmTask, 1000, 3);
    //HAL_Delay(1);
    initRTOS();
    //}
}


#endif