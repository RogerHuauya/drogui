//#define ICM_20948_TEST
#ifdef ICM_20948_TEST
#include "ICM20948.h"
#include "controlTasks.h"
#include "sensorsTasks.h"
#include "mahony.h"
#include "task.h"
#include "debugTasks.h"
#include "MPU9250.h"
#include "serial.h"
#include "_main.h"

/*icm20948 myicm;
mpu9250 mympu;*/

mahony mahonyICM;
mahony mahonyMPU;

void mpuTask(){
    
    //readMpuGyro(&myMPU);
    //readMpuAcc(&myMPU);
    
    readIcmRawGyro(&myICM);
    readMpuRawGyro(&myMPU);

    float val1 = myICM.raw_gx/131 + myICM.off_gx;
    float val2 = myMPU.raw_gx/65.5 + myMPU.off_gx;
    
    serialPrintf("%f\t", val1);
    val1 = computeFilter(&(myICM.fGyroX.first), val1);
    serialPrintf("%f\t", val1);
    ///val = computeFilter(&(myICM.fGyroX.second), val);
    //serialPrintf("%f\t", val);
    val1 = computeDNotch(&(myICM.fGyroX.third), val1);
    serialPrintf("%f\n", val1);
    //val = computeDNotch(&(myICM.fGyroX.fourth), val);
    //serialPrintf("%f\n", val);
    // serialPrint("loop\n");
    
}
float rpyICM[3];
float rpyMPU[3];

float icm_roll, icm_pitch, icm_yaw;
float mpu_roll, mpu_pitch, mpu_yaw;

float conv = 180.0/PI;

void mahonyCompareTask(){

    mahonyUpdate(&mahonyICM, myICM.gx*PI/180.0, myICM.gy*PI/180.0, myICM.gz*PI/180.0,myICM.ax, myICM.ay, myICM.az, myMPU.my, myMPU.mx, -myMPU.mz);
    getMahonyEuler(&mahonyICM, rpyICM);

    icm_roll = conv*rpyICM[0], icm_pitch = conv*rpyICM[1], icm_yaw = conv*rpyICM[2];

    mahonyUpdate(&mahonyMPU, myMPU.gx*PI/180.0, myMPU.gy*PI/180.0, myMPU.gz*PI/180.0,myMPU.ax, myMPU.ay, myMPU.az, myMPU.my, myMPU.mx, -myMPU.mz);
    getMahonyEuler(&mahonyMPU, rpyMPU);

    mpu_roll = conv*rpyMPU[0], mpu_pitch = conv*rpyMPU[1], mpu_yaw = conv*rpyMPU[2];

}

void dbgTask(){

    serialPrintf("%f\t%f\n", icm_roll, mpu_roll);
}


void _main(){
    
    HAL_Delay(1000);
    
    initMahony(&mahonyICM, 2, 0.1, 500);
    initMahony(&mahonyMPU, 2, 0.1, 500);
    
    initIcm(&myICM);
    initMpu(&myMPU);

    initDebug();
    initControlTasks();
    //initSensorsTasks();
    addTask(&mpuTask, 1000, 3);
    //addTask(&dbgTask, 10000, 0);

    serialPrint("Calibrate Gyro icm...");
    calibrateIcmGyro(&myICM);
    serialPrint("Done!\n");
    
    serialPrint("Calibrate Gyro mpu...");
    calibrateMpuGyro(&myMPU);
    serialPrint("Done!\n");
    
    
    /*serialPrint("Calibrate Accel icm...");
    calibrateIcmAccel(&myICM);
    serialPrint("Done!\n");
    
    serialPrint("Calibrate Accel mpu...");
    calibrateMpuAccel(&myMPU);
    serialPrint("Done!\n");
    */
    /*serialPrint("Calibrate mag...");
    calibrateMpuMag(&myMPU);
    serialPrint("Done!\n");*/
    
    //addTask(&mahonyCompareTask, 2000, 2);
    initRTOS();
    
}


#endif