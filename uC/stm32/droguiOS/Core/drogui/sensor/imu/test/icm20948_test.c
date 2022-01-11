//#define ICM_20948_TEST
#ifdef ICM_20948_TEST
#include "macros.h"
#include "ICM20948.h"
#include "mahony.h"
#include "task.h"
#include "serial.h"
#include "_main.h"

icm20948 myimu;
mahony mahonyICM;

void mpuTask(){
    readAcc(&myimu);
    readGyro(&myimu);
}

float conv = 180.0/PI;
float rpyICM[3];

void mahonyTask(){
    mahonyUpdate(&mahonyICM, myimu.gx*PI/180.0, myimu.gy*PI/180.0, myimu.gz*PI/180.0,myimu.ax, myimu.ay, myimu.az, myimu.my, myimu.mx, -myimu.mz);
    getMahonyEuler(&mahonyICM, rpyICM);
}

void dbgTask(){
    serialPrintf(SER_DBG, "%.3f %.3f %.3f\n", myimu.gx, myimu.gy, myimu.gz);
}


void _main(){
    HAL_Delay(1000);
    serialsBegin();
    initImu(&myimu);
    initMahony(&mahonyICM, 2, 0.1, 500);
    addTask(&dbgTask, 10000, 1);
    addTask(&mpuTask, 1000, 3);
    initRTOS();
}


#endif
