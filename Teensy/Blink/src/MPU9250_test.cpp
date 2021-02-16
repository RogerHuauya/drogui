#define MPU9250_TEST
#ifdef MPU9250_TEST

#include "..\headers\timer.h"
#include "..\headers\main.h"
#include "..\headers\MPU9250.h"
#include <Arduino.h>

mpu9250 myIMU;
timer timer_accel, timer_gyro;

void accelInterrupt(){
    readAcc(&myIMU);/*
    Serial.print(myIMU.ax);
    Serial.print("\t");
    Serial.print(myIMU.ay);
    Serial.print("\t");
    Serial.print(myIMU.az);
    Serial.print("\t");*/
}

void gyroInterrupt(){
    readGyro(&myIMU);/*
    Serial.print(myIMU.gx);
    Serial.print("\t");
    Serial.print(myIMU.gy);
    Serial.print("\t");*/
    Serial.print(myIMU.gz);
    Serial.print("\n");
}

void mainInterrupt(){
    Serial.println("main was here");
}

void initializeSystem(){

    initMpu(&myIMU);
    calibrateGyro(&myIMU);
    Serial.println("Gyro calibrated ...!!");
    //calibrateAccel(&myIMU);
    //Serial.println("Accel calibrated ...!!");
    initTimer(&timer_accel, &accelInterrupt, 1000);
    initTimer(&timer_gyro, &gyroInterrupt, 1000);
    delay(500);
}


int _main(void){

    initializeSystem();
    delay(1000);
    while(1){
        if(timerReady(&timer_accel)) executeTimer(&timer_accel);
        if(timerReady(&timer_gyro))  executeTimer(&timer_gyro);
        //if(timerReady(&timer_main)) executeTimer(&timer_main);
    }
    return 0;
}
#endif
