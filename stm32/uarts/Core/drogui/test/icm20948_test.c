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

icm20948 myimu;

mahony mahonyICM;

void mpuTask(){
    
    readAcc(&myimu);
    readGyro(&myimu);
    
}
float rpyICM[3];
float rpyMPU[3];

float icm_roll, icm_pitch, icm_yaw;
float mpu_roll, mpu_pitch, mpu_yaw;

float conv = 180.0/PI;

void mahonyCompareTask(){

    mahonyUpdate(&mahonyICM, myimu.gx*PI/180.0, myimu.gy*PI/180.0, myimu.gz*PI/180.0,myimu.ax, myimu.ay, myimu.az, myimu.my, myimu.mx, -myimu.mz);
    getMahonyEuler(&mahonyICM, rpyICM);

    icm_roll = conv*rpyICM[0], icm_pitch = conv*rpyICM[1], icm_yaw = conv*rpyICM[2];



}

void dbgTask(){

    serialPrintf(&serial3, "%.3f %.3f %.3f\n", myimu.ax, myimu.ay, myimu.az);
}


void _main(){
    
    HAL_Delay(1000);
    serialsBegin();
    //initMahony(&mahonyICM, 2, 0.1, 500);
    initImu(&myimu);
    addTask(&dbgTask, 10000, 1);
    addTask(&mpuTask, 1000, 3);

    initRTOS();
    
}


#endif