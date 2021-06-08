//#define MAHONY_TEST
#ifdef MAHONY_TEST

#define MAGCALIB_TEST
#ifdef MAGCALIB_TEST

#include "..\headers\timer.h"
#include "..\headers\main.h"
#include "..\headers\matlib.h"
#include "..\headers\MPU9250.h"
#include "..\headers\sensorsTasks.h"
#include "..\headers\mahony.h"
#include <Arduino.h>


timer timer_debug;
float rpy[3];

void debugInterrupt(){
    mahonyUpdate(gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f, ax, ay, az, my, mx, mz);
    getMahonyEuler(rpy);

    Serial.print(rpy[0]);
    Serial.print('\t');
    Serial.print(rpy[1]);
    Serial.print('\t');
    Serial.print(rpy[2]);
    Serial.print('\n');
}

int _main(void){
    initTimer(&timer_debug, &debugInterrupt, 100);
    initSensorsTasks();
    
    while(1){
        executeSensorsTasks();
        if(timerReady(&timer_debug)) executeTimer(&timer_debug);
    }

    return 0;
}
#endif


#endif