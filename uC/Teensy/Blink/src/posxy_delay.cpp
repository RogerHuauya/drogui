#define POSXY_DELAY
#ifdef POSXY_DELAY

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

    char c;

    if(Serial.available()){
        isDebugOn = true;
        c = Serial.read();
    }


    if(!isDebugOn) return;

    /*Serial.print(M1);
    Serial.print("\t");
    Serial.print(M2);
    Serial.print("\t");
    Serial.print(M3);
    Serial.print("\t");
    Serial.print(M4);
    Serial.print("\n");*/

    if( c == 'a' ) setReg(CAL_GYR_TRG,1);
    if( c == 'b' ) setReg(CAL_ACC_TRG,1);
    if( c == 'c' ) setReg(CAL_MAG_TRG,1);
    if( c == 'k' ) setReg(START,1);

    /*
    Serial.print(ax,DEC);
    Serial.print("\t");
    Serial.print(ay,DEC);
    Serial.print("\t");
    Serial.print(az,DEC);
    Serial.print("\t");
    */
    /*
    Serial.print(gx,DEC);
    Serial.print("\t");
    Serial.print(gy,DEC);
    Serial.print("\t");
    Serial.print(gz,DEC);
    Serial.print("\t");
    */
    /*
    Serial.print(wroll_control.errd,DEC);
    Serial.print("\t");
    Serial.print(wpitch_control.errd,DEC);*/
    /*
    Serial.print(mx,DEC);
    Serial.print("\t");
    Serial.print(my,DEC);
    Serial.print("\t");
    Serial.print(mz,DEC);
    Serial.print("\n");
    */
   /*
    Serial.print(roll*180/pi,DEC);
    Serial.print("\t");
    Serial.print(pitch*180/pi,DEC);
    Serial.print("\t");
    Serial.print(yaw*180/pi,DEC);
    Serial.print("\t");
    */
    
    Serial.print(x*10,DEC);
    Serial.print("\t");
    Serial.print(y*10,DEC);
    Serial.print("\t");
    
    Serial.print(getReg(GPS_X)*10, DEC);
    Serial.print("\t");
    Serial.print(getReg(GPS_Y)*10, DEC);
    //Serial.print("\t");
    //Serial.print(z,DEC);
    Serial.print("\n");

}


void securityInterrupt(){
    if(getReg(Z_REF) == 0 /*|| (fabs(angle_dif(roll_ref, roll))> pi/9) || (fabs(angle_dif(pitch_ref, pitch))> pi/9)*/){
        updatePID();
        if(getReg(CAL_GYR_TRG) == 1) calibrateGyro(&myIMU), setReg(CAL_GYR_TRG, 0);
        if(getReg(CAL_ACC_TRG) == 1) calibrateAccel(&myIMU), setReg(CAL_ACC_TRG, 0);
        if(getReg(CAL_MAG_TRG) == 1) calibrateMag(&myIMU), setReg(CAL_MAG_TRG, 0);
        security = true;
    }
    else security = false;
}


void initializeSystem(){
    
    initI2C(SLAVE, I2C1, 0x60);
    clearI2Cregisters(I2C1);
    initTimer(&timer_security, &securityInterrupt, 100);
    initTimer(&timer_debug, &debugInterrupt, 100);
    initTimer(&timer_blink, &blinkInterrupt, 10);


    delay(500);
}


int _main(void){

    initializeSystem();
    initSensorsTasks();
    
    delay(1000);
    while(1){
        if(timerReady(&timer_blink)) executeTimer(&timer_blink);
        if(timerReady(&timer_debug)) executeTimer(&timer_debug);
        //if(timerReady(&timer_security)) executeTimer(&timer_security);  
        executeSensorsTasks();   
    }
    return 0;
}
#endif