#define SENSORS
#ifdef SENSORS

#include "..\headers\main.h"
#include "..\headers\kalman.h"
#include "..\headers\control.h"
#include "..\headers\pwm.h"
#include "..\headers\i2c.h"
#include "..\headers\utils.h"
#include "..\headers\timer.h"
#include "..\headers\registerMap.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>
#include <Adafruit_BMP3XX.h>


#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define N 25

#define SEALEVELPRESSURE_HPA (1013.25)

char buffer[80];

i2c slave;

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
Adafruit_BMP3XX bmp;

float alt_memo[N], alt_fast, alt_slow = 0, alt_diff, sum = 0,alt_offs;
int alt_pointer = 0;
float sealevel;

timer timer_sensors, timer_main;

volatile double roll, pitch, yaw, ax, ay, az;
float x, y, z;
volatile unsigned long long time = 0;
bool led_state;

void sensorsInterrupt(){
	
    
    digitalWrite(LED_BUILTIN, led_state);
    led_state = !led_state;

    sensors_event_t orientationData , linearAccelData;
    
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    Serial.print(accel.x());
    Serial.print("\t");
    Serial.print(accel.y());
    Serial.print("\t");
    Serial.print(accel.z());
    Serial.print("\n");
    /*
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    
    yaw = (float)orientationData.orientation.x*pi/180 - pi;
    pitch = (float)orientationData.orientation.y*pi/180;
    roll = (float)orientationData.orientation.z*pi/180;

    setReg(ROLL_VAL,(float)(roll));
    setReg(PITCH_VAL,(float)(pitch));
    setReg(YAW_VAL,(float)(yaw));


    alt_pointer %= N;
    sum -= alt_memo[alt_pointer];
    alt_memo[alt_pointer] = bmp.readAltitude(sealevel);
    sum += alt_memo[alt_pointer++];
    alt_fast = sum / N;

    alt_slow = alt_slow * 0.99 + alt_fast * 0.01;

    alt_diff = alt_fast - alt_slow;
    alt_diff = max(min(alt_diff, 1), -1);

    if (abs(alt_diff) >  0.20) alt_slow += alt_diff / 6.0;
    
    Serial.print(roll);
    Serial.print("\t");
    Serial.print(pitch);
    Serial.print("\t");
    Serial.print(yaw);
    Serial.print("\t");
    
    Serial.println(alt_slow - alt_offs);    
    */
}

void mainInterrupt(){
    Serial.println("main was here");
}

void initializeSystem(){
    
    Serial.println("Hola 2");

    if(!bno.begin()){

        Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while(1){Serial.println("Bno");delay(10);}
    }
    
    delay(1000);

    bno.setExtCrystalUse(true);
    
    initI2C(SLAVE, I2C1, 0x60);
    clearI2Cregisters(I2C1);
     
    if (!bmp.begin_I2C()) {  
        Serial.println("Could not find a valid BMP3 sensor, check wiring!");
        while (1){ Serial.println("BMP"); delay(1000);};
    }

    Serial.println("Hola 3");

    bmp.setTemperatureOversampling(BMP3_NO_OVERSAMPLING);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_2X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_DISABLE);
    bmp.setOutputDataRate(BMP3_ODR_100_HZ);
    sealevel = bmp.readPressure() / 100.0;

    for ( int i = 0; i < N; i++ ) alt_memo[i] = bmp.readAltitude(sealevel), sum += alt_memo[i] / N, delay(5);

    alt_slow = sum / N;
    alt_offs = alt_slow ;

    initTimer(&timer_sensors, &sensorsInterrupt, 1000);
    initTimer(&timer_main, &mainInterrupt, 1);

    delay(500);
    
}


bool caso = true;

long long entrada = 0;
int dig = 0;

double roll_off = 0 , pitch_off = 0, yaw_off = 0, x_off = 0, y_off = 0, z_off = 0;
double roll_ref, pitch_ref, yaw_ref, x_ref, y_ref, z_ref;
long long pm = 0;


int _main(void){

    initializeSystem();
    delay(1000);
    while(1){
        if(timerReady(&timer_sensors)) executeTimer(&timer_sensors);

        if(timerReady(&timer_main)) executeTimer(&timer_main);
    }
    return 0;
}
#endif
