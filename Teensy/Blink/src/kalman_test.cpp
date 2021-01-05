#define KALMAN_TEST
#ifdef KALMAN_TEST

#include "..\headers\main.h"
#include "..\headers\pwm.h"
#include "..\headers\i2c.h"
#include "..\headers\utils.h"
#include "..\headers\registerMap.h"
#include "..\headers\kalman.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>


char buffer[80];

i2c slave;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

IntervalTimer readSensors;
IntervalTimer milli;

volatile double roll, pitch, yaw, ax, ay, az;
float x, y, z;
volatile unsigned long long time = 0;
bool led_state;

void timer1Interrupt(){

    digitalWrite(LED_BUILTIN, HIGH);
    led_state = !led_state;

    sensors_event_t orientationData , linearAccelData;
    
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    
    yaw = (float)orientationData.orientation.x*pi/180 - pi;
    pitch = (float)orientationData.orientation.y*pi/180;
    roll = (float)orientationData.orientation.z*pi/180;
    
    setReg(ROLL_VAL,(float)(roll));
    setReg(PITCH_VAL,(float)(pitch));
    setReg(YAW_VAL,(float)(yaw));

    ax = linearAccelData.acceleration.x;
    ay = linearAccelData.acceleration.y;
    az = linearAccelData.acceleration.z;

    setReg(ACC_X,(float)(ax));
    setReg(ACC_Y,(float)(ay));
    setReg(ACC_Z,(float)(az));

    uint8_t sys, gyro, accel, mag = 0;
    bno.getCalibration(&sys, &gyro, &accel, &mag);

    setReg(CAL_SYS, (float) sys);
    setReg(CAL_GYR, (float) gyro);
    setReg(CAL_ACC, (float) accel);
    setReg(CAL_MAG, (float) mag);


    if(getReg(START) > 0){
        kalmanUpdateIMU(ax, ay, az, roll, pitch, yaw);
        if(getReg(GPS_AVAILABLE) == 1) setReg(GPS_AVAILABLE, 0), kalmanUpdateGPS(getReg(GPS_X), getReg(GPS_Y), 0);    
    }
    else{
        clearKalman();
    }

    getPosition(&x, &y, &z);
    
    setReg(X_VAL, x);
    setReg(Y_VAL, y);
    setReg(Z_VAL, z);
    Serial.print(x), Serial.print('\t'),
    Serial.print(y), Serial.print('\n'); 
}

void timer2Interrupt(){
    time++;
}

void initializeSystem(){

    if(!bno.begin()){

        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1);
    }
    
    delay(1000);

    bno.setExtCrystalUse(true);

    readSensors.begin(timer1Interrupt, 10000);
    readSensors.priority(0);
    

    setKalmanTsImu(0.01);
    setKalmanTsGps(1);
    initMatGlobal();

    initI2C(SLAVE, I2C1, 0x60);
    clearI2Cregisters(I2C1);

    delay(500);
    
}


int main(void){
  
    initializeSystem();

    while(1){
        delay(20);
    }
    return 0;
}
#endif
