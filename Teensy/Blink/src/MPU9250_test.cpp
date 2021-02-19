//#define MPU9250_TEST
#ifdef MPU9250_TEST

#include "..\headers\timer.h"
#include "..\headers\main.h"
#include "..\headers\MPU9250.h"
#include <Arduino.h>

mpu9250 _myIMU;
timer _timer_accel, _timer_gyro, _timer_mag;

void _accelInterrupt(){
    readAcc(&_myIMU);/*
    Serial.print(myIMU.ax);
    Serial.print(" ");
    Serial.print(myIMU.ay);
    Serial.print(" ");
    Serial.print(myIMU.az);
    Serial.print(" ");*/
}

void _gyroInterrupt(){
    readGyro(&_myIMU);/*
    Serial.print(myIMU.gx);
    Serial.print(" ");
    Serial.print(myIMU.gy);
    Serial.print(" ");
    Serial.print(myIMU.gz);
    Serial.print("\n");*/
}

void _magInterrupt(){
    readRawMag(&_myIMU);/*
    Serial.print(myIMU.raw_mx);
    Serial.print(" ");
    Serial.print(myIMU.raw_my);
    Serial.print(" ");
    Serial.print(myIMU.raw_mz);


    Serial.print("\n");*/
}

void _mainInterrupt(){
    Serial.println("main was here");
}

void initializeSystem(){

    initMpu(&_myIMU);
    
    calibrateGyro(&_myIMU);
    Serial.println("Gyro calibrated ...!!");
    
    /*calibrateAccel(&myIMU);
    Serial.println("Accel calibrated ...!!");
    */
    /*calibrateMag(&myIMU);
    Serial.println("Mag calibrated ...!!");
    */
    initTimer(&_timer_accel, &_accelInterrupt, 1000);
    initTimer(&_timer_gyro, &_gyroInterrupt, 1000);
    //initTimer(&_timer_mag, &_magInterrupt, 10);
    delay(500);
}


int _main(void){
    
    initializeSystem();
    delay(1000);
    while(1){
        if(timerReady(&_timer_accel))  Serial.print("Accel: "), Serial.println(_timer_accel.time), executeTimer(&_timer_accel);
        if(timerReady(&_timer_gyro))   Serial.print("Gyro: "), Serial.println(_timer_gyro.time),executeTimer(&_timer_gyro);
        //if(timerReady(&_timer_mag))  Serial.print("Mag: "), Serial.println(_timer_mag.time),executeTimer(&_timer_mag);
    }
    
    return 0;
}
#endif
