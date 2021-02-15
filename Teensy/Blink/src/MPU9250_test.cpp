#define MPU9250_TEST
#ifdef MPU9250_TEST

#include "..\headers\timer.h"
#include "..\headers\main.h"
#include "..\headers\i2c.h"
#include "MPU9250.h"
#include <Arduino.h>
//#include <i2c_driver.h>
//#include <i2c_driver_wire.h>

#define I2Cclock 400000
#define I2Cport Wire
//#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0   // Use either this line or the next to select which I2C address your device is using
#define MPU9250_ADDRESS MPU9250_ADDRESS_AD1

MPU9250 myIMU(MPU9250_ADDRESS, I2Cport, I2Cclock);
timer timer_sensors, timer_main;
// Pin definitions
int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins
int myLed  = 13;  // Set up pin 13 led for toggling

volatile unsigned long long time = 0;
bool led_state = false;
void sensorsInterrupt(){
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;
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
    
    Wire.begin();
    // TWBR = 12;  // 400 kbit/sec I2C speed
    Serial.begin(38400);

    while(!Serial){};

    // Set up the interrupt pin, its set as active high, push-pull
    pinMode(intPin, INPUT);
    digitalWrite(intPin, LOW);
    pinMode(myLed, OUTPUT);
    digitalWrite(myLed, HIGH);

    byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    Serial.print(F("MPU9250 I AM 0x"));
    Serial.print(c, HEX);
    Serial.print(F(" I should be 0x"));
    Serial.println(0x71, HEX);

    if (c == 0x71) {// WHO_AM_I should always be 0x71
        Serial.println(F("MPU9250 is online..."));

        // Start by performing self test and reporting values
        myIMU.MPU9250SelfTest(myIMU.selfTest);
        Serial.print(F("x-axis self test: acceleration trim within : "));
        Serial.print(myIMU.selfTest[0],1); Serial.println("% of factory value");
        Serial.print(F("y-axis self test: acceleration trim within : "));
        Serial.print(myIMU.selfTest[1],1); Serial.println("% of factory value");
        Serial.print(F("z-axis self test: acceleration trim within : "));
        Serial.print(myIMU.selfTest[2],1); Serial.println("% of factory value");
        Serial.print(F("x-axis self test: gyration trim within : "));
        Serial.print(myIMU.selfTest[3],1); Serial.println("% of factory value");
        Serial.print(F("y-axis self test: gyration trim within : "));
        Serial.print(myIMU.selfTest[4],1); Serial.println("% of factory value");
        Serial.print(F("z-axis self test: gyration trim within : "));
        Serial.print(myIMU.selfTest[5],1); Serial.println("% of factory value");

        // Calibrate gyro and accelerometers, load biases in bias registers
        myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);

        myIMU.initMPU9250();
        // Initialize device for active mode read of acclerometer, gyroscope, and
        // temperature
        Serial.println("MPU9250 initialized for active data mode....");

        // Read the WHO_AM_I register of the magnetometer, this is a good test of
        // communication
        byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
        Serial.print("AK8963 ");
        Serial.print("I AM 0x");
        Serial.print(d, HEX);
        Serial.print(" I should be 0x");
        Serial.println(0x48, HEX);

        if (d != 0x48){
        // Communication failed, stop here
        Serial.println(F("Communication failed, abort!"));
        Serial.flush();
        abort();
        }

        // Get magnetometer calibration from AK8963 ROM
        myIMU.initAK8963(myIMU.factoryMagCalibration);
        // Initialize device for active mode read of magnetometer
        Serial.println("AK8963 initialized for active data mode....");

    // Get sensor resolutions, only need to do this once
        myIMU.getAres();
        myIMU.getGres();
        myIMU.getMres();

        // The next call delays for 4 seconds, and then records about 15 seconds of
        // data to calculate bias and scale.
    //    myIMU.magCalMPU9250(myIMU.magBias, myIMU.magScale);
        Serial.println("AK8963 mag biases (mG)");
        Serial.println(myIMU.magBias[0]);
        Serial.println(myIMU.magBias[1]);
        Serial.println(myIMU.magBias[2]);

        Serial.println("AK8963 mag scale (mG)");
        Serial.println(myIMU.magScale[0]);
        Serial.println(myIMU.magScale[1]);
        Serial.println(myIMU.magScale[2]);
    //    delay(2000); // Add delay to see results before serial spew of data
    } // if (c == 0x71)

    else{
        Serial.print("Could not connect to MPU9250: 0x");
        Serial.println(c, HEX);

        // Communication failed, stop here
        Serial.println(F("Communication failed, abort!"));
        Serial.flush();
        abort();
    }
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
