//#define SENSORS
#ifdef SENSORS

#include "..\headers\timer.h"
#include "..\headers\main.h"
#include "..\headers\i2c.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
timer timer_sensors, timer_main;

volatile unsigned long long time = 0;
bool led_state = false;
void sensorsInterrupt(){
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
    
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    return;
    /*Serial.print("\t");
    Serial.print(accel.x());
    Serial.print("\t");
    Serial.print(accel.y());
    Serial.print("\t");
    Serial.print(accel.z());*/
    //Serial.print("\n");
}

void mainInterrupt(){
    Serial.println("main was here");
}

void initializeSystem(){
    
    if(!bno.begin()){

        Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1){Serial.println("Bno");delay(10);}
    }
    
    delay(1000);

    bno.setExtCrystalUse(true);
    initTimer(&timer_sensors, &sensorsInterrupt, 1000);
    //initTimer(&timer_main, &mainInterrupt, 1);

    delay(500);
    
}


int _main(void){

    initializeSystem();
    delay(1000);
    while(1){
        if(timerReady(&timer_sensors)) Serial.println(timer_sensors.time), executeTimer(&timer_sensors);

        //if(timerReady(&timer_main)) executeTimer(&timer_main);
    }
    return 0;
}
#endif
