#define SAVGOL_TEST
#ifdef SAVGOL_TEST

#include "_main.h"
#include "tim.h"
#include "sensorsTasks.h"
#include "filter.h"
#include "debugTasks.h"
#include "task.h"
#include "serial.h"
char auxbuff[50];

savGolDFilter sgd;


void savGolayTask(){
    float ret = computeSavGolDFilter(&sgd, gx);
    serialPrintf("%f %f\n", ret, gx);
}


void _main(){


    HAL_Delay(1000);
    
    #if PORT == DEBUG
        serialPrint("Start System\n");
    #endif

    initSavGolDFilter(&sgd, 7);

    initSensorsTasks();
    addTask(&savGolayTask, 1000, 0);
    initRTOS();
}

#endif