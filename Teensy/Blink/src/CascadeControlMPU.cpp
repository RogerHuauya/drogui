//#define CASCADE_CONTROL_MPU_TEST
#ifdef CASCADE_CONTROL_MPU_TEST

#include "..\headers\timer.h"
#include "..\headers\main.h"
#include "..\headers\utils.h"
#include "..\headers\registerMap.h"
#include "..\headers\i2c.h"
#include "..\headers\controlTasks.h"
#include "..\headers\sensorsTasks.h"
#include <Arduino.h>


i2c slave;


bool led_state;

timer timer_blink, timer_debug, timer_security;
  
void blinkInterrupt(){
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
}

bool isDebugOn = false;
void debugInterrupt(){

    if(Serial.available()) isDebugOn = true;

    if(!isDebugOn) return;

    /*Serial.print(M1);
    Serial.print("\t");
    Serial.print(M2);
    Serial.print("\t");
    Serial.print(M3);
    Serial.print("\t");
    Serial.print(M4);
    Serial.print("\n");*/

    Serial.print((int) gx);
    Serial.print("\t");
    Serial.print((int) gy);
    Serial.print("\t");
    Serial.print((int)gz);
    Serial.print("\n");

    
}


void securityInterrupt(){
    if(getReg(Z_REF) == 0 /*|| (fabs(angle_dif(roll_ref, roll))> pi/9) || (fabs(angle_dif(pitch_ref, pitch))> pi/9)*/){
        updatePID();
        security = true;
    }
    else security = false;
}


void initializeSystem(){
    
    initI2C(SLAVE, I2C1, 0x60);
    clearI2Cregisters(I2C1);
    initTimer(&timer_security, &securityInterrupt, 100);
    initTimer(&timer_debug, &debugInterrupt, 1000);
    initTimer(&timer_blink, &blinkInterrupt, 10);


    delay(500);
}


int _main(void){

    initializeSystem();
    initControlTasks();
    initSensorsTasks();


    delay(1000);
    while(1){
        if(timerReady(&timer_blink)) executeTimer(&timer_blink);
        if(timerReady(&timer_debug)) executeTimer(&timer_debug);
        if(timerReady(&timer_security)) executeTimer(&timer_security);  
        executeControlTasks();
        executeSensorsTasks();   
    }
    return 0;
}
#endif