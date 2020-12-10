//#define MPU_TEST
#ifdef MPU_TEST

#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"
#define LED PRTC, 8
#define ADD 68
int AccelX = 0;

#define SMPLRT_DIV         0x19
#define CONFIG             0x1A
#define GYRO_CONFIG        0x1B
#define ACCEL_CONFIG       0x1C
#define PWR_MGMT_1         0x6B

char s[50];
int s1, s2, s3;
int main(){
    initConfig();
    initSerial(9600);
    initI2C();
    delayMs(1000);

    pinMode(LED, OUTPUT);


    // Setting The Sample (Data) Rate
    
    serialWriteString("Starting\n");
    i2cStart(); 
    serialWriteString("Started\n");
    s1 = i2cWrite(ADD << 1);
    s2 = i2cWrite(SMPLRT_DIV);
    s3 = i2cWrite(0x07);
    i2cStop();
    
    sprintf(s,"SMPLRT_DIV: %d %d %d\n",s1,s2,s3);
    serialWriteString(s);
    // Setting The Clock Source
    
    i2cStart(); 
    s1 = i2cWrite(ADD << 1);
    s2 = i2cWrite(PWR_MGMT_1);
    s3 = i2cWrite(0x00);
    i2cStop();

    sprintf(s,"PWR_MGMT_1: %d %d %d\n",s1,s2,s3);
    serialWriteString(s);

    // Configure The DLPF
    i2cStart(); 
    s1 = i2cWrite(ADD << 1);
    s2 = i2cWrite(CONFIG);
    s3 = i2cWrite(0x00);
    i2cStop();
    
    sprintf(s,"CONFIG: %d %d %d\n",s1,s2,s3);
    serialWriteString(s);
    
    // Configure The ACCEL (FSR= +-2g)
    i2cStart(); 
    s1 = i2cWrite(ADD << 1);
    s2 = i2cWrite(ACCEL_CONFIG);
    s3 = i2cWrite(0x00);
    i2cStop();
    
    sprintf(s,"ACCEL_CONFIG: %d %d %d\n",s1,s2,s3);
    serialWriteString(s);
    
    serialWriteString("Configurado\n");


    while(1){
        
        i2cStart();
        s1 = i2cWrite(ADD << 1);
        s2 = i2cWrite(0x3B);
        sprintf(s,"address: %d %d\n",s1,s2);
        serialWriteString(s);
        
        i2cStop();
        i2cStart();

        s1 = i2cWrite((ADD << 1) | 1);
        sprintf(s,"read: %d\n",s1);
        serialWriteString(s);  

        AccelX = ((int)i2cRead(1)<<8); i2cSendACK();
        AccelX |= ((int)i2cRead(0)); i2cSendNACK();
        i2cStop();

        sprintf(s,"%f\n",(float)AccelX/16384.0);
        serialWriteString(s);
        digitalWrite(LED, 1);
        delayMs(50);
        digitalWrite(LED, 0);
        delayMs(50);
        //serialWriteString(s);
    }
    return 0;
}
#endif  
