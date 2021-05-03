#define ICM_20948_TEST
#ifdef ICM_20948_TEST
#include "ICM20948.h"
#include "MPU9250.h"
#include "serial.h"
#include "_main.h"

icm20948 myicm;
mpu9250 mympu;

void _main(){
    
    HAL_Delay(1000);
    serialPrint("Starting...\n");
    initIcm(&myicm);
    initMpu(&mympu);
    serialPrint("ICM started...!\n");
    calibrateIcmGyro(&myicm);
    calibrateMpuGyro(&mympu);
    while(1){
        readIcmRawGyro(&myicm);
        
        float val = myicm.raw_gx/65.5 + myicm.off_gx;
        //serialPrintf("%f\t", val);
        val = computeFilter(&(myicm.fGyroX.first), val);
        serialPrintf("%f\t", val);
        val = computeFilter(&(myicm.fGyroX.second), val);
        serialPrintf("%f\t", val);
        val = computeDNotch(&(myicm.fGyroX.third), val);
        serialPrintf("%f\n", val);
        val = computeDNotch(&(myicm.fGyroX.fourth), val);
        //serialPrintf("%f\n", val);


        // serialPrint("loop\n");
        HAL_Delay(1);
    }
}


#endif