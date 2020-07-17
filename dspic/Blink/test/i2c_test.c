#define I2C_TEST
#ifdef I2C_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"
#define LED PRTB, 4
#define add 0x68
#define SMPLRT_DIV         0x19
#define CONFIG             0x1A
#define GYRO_CONFIG        0x1B
#define ACCEL_CONFIG       0x1C
#define PWR_MGMT_1         0x6B

long long AccelX = 0;

char s[50];

int main(){
    initConfig();
    initSerial();
    initI2C();
    pinMode(LED, OUTPUT);


    // Setting The Sample (Data) Rate
    i2cStart(); i2cWrite(add << 1);
    i2cWrite(SMPLRT_DIV);
    i2cWrite(0x07);
    i2cStop();
    
    /*
    // Setting The Clock Source
    i2cStart(); i2cWrite(add << 1);
    i2cWrite(PWR_MGMT_1);
    i2cWrite(0x00);
    i2cStop();

    // Configure The DLPF
    i2cStart(); i2cWrite(add << 1);
    i2cWrite(CONFIG);
    i2cWrite(0x00);
    i2cStop();
    
    // Configure The ACCEL (FSR= +-2g)
    i2cStart(); i2cWrite(add << 1);
    i2cWrite(ACCEL_CONFIG);
    i2cWrite(0x00);
    i2cStop();*/

    while(1){
        /*i2cStart();
        i2cWrite(add << 1);
        i2cWrite(0x3D);
        i2cRestart();
        i2cWrite((add << 1) | 1);   
        AccelX = (i2cRead(1) << 8);
        __delay_ms(1);
        AccelX |= (i2cRead(0));
        i2cStop();
        sprintf(s,"AccelX: %lld\n",AccelX);
        serialWriteString(s);*/
        digitalWrite(LED, 1);
        __delay_ms(500);
        digitalWrite(LED, 0);
        __delay_ms(500);
    }
    return 0;
}
#endif  
