#define ICM_20948_TEST
#ifdef ICM_20948_TEST
#include "ICM20948.h"
#include "MPU9250.h"
#include "serial.h"
#include "_main.h"

icm20948 myicm;
mpu9250 mympu;

void _main(){
    
    serialPrint("Starting...");
    HAL_Delay(1000);
    serialPrint("Starting...");
    initIcm(&myicm);
    initMpu(&mympu);
    serialPrint("ICM started...!");
    calibrateIcmGyro(&myicm);
    while(1){
        readIcmGyro(&myicm);
        serialPrintf("%f\t%f\t%f\n", myicm.gx, myicm.gy, myicm.gz);
                                                 //myicm.raw_gx, myicm.raw_gy, myicm.raw_gz);
        // serialPrint("loop\n");
        HAL_Delay(1);
    }
}


#endif