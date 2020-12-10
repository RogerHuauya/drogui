// #define SFM3000
#ifdef SFM3000

#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"

#define LED PRTD, 9
#define ADD 64
#define READ_CMD 0xF1
#define START_MSB 0x10
#define START_LSB 0X00

short press = 0;
char crc = 0;
char s[50];
int s1, s2, s3;

i2c sfm;

unsigned char c1,c2,c3;
int main(){

    
    initPBCLK();
    initSerial(115200, 4);
    initI2C(&sfm, I2C1, ADD, 100000);
    delayMs(1000);

    pinMode(LED, OUTPUT);
    serialWriteString("Hola mundo\n");
    digitalWrite(LED, 1);

    delayMs(100);
    
    i2cStart(&sfm);
    s1  = i2cWrite(&sfm, ADD << 1);
    s2  = i2cWrite(&sfm, 0x31);
    s3  = i2cWrite(&sfm, 0xAE);

    sprintf(s, "Start: %d %d %d \n", s1, s2 ,s3);
    serialWriteString(s);

    
    delayMs(100);


    i2cStart(&sfm);
    s1  = i2cStartRead(&sfm);
    
    int a = 0, i;
    for(i = 0; i < 6; i++){
        c1 = i2cRead(&sfm); 
        if(i < 5) i2cSendACK(&sfm);
        else i2cSendNACK(&sfm);
        if(i == 2 || i == 5) continue;
        a <<= 8; 
        a |= c1;
    }
    //i2cStop(&sfm);

    sprintf(s, "Serial number: %d\n", a);
    serialWriteString(s);
    i2cStop(&sfm);

    
    delayMs(100);


    i2cStart(&sfm);
    s1  = i2cWrite(&sfm, ADD << 1);
    s2  = i2cWrite(&sfm, 0x31);
    s3  = i2cWrite(&sfm, 0xAE);

    sprintf(s, "Start: %d %d %d \n", s1, s2 ,s3);
    serialWriteString(s);

    
    delayMs(100);


    i2cStart(&sfm);
    s1  = i2cStartRead(&sfm);
    
    a = 0;
    for(i = 0; i < 6; i++){
        c1 = i2cRead(&sfm); 
        if(i < 5) i2cSendACK(&sfm);
        else i2cSendNACK(&sfm);
        if(i == 2 || i == 5) continue;
        a <<= 8; 
        a |= c1;
    }
    //i2cStop(&sfm);

    sprintf(s, "Serial number: %d\n", a);
    serialWriteString(s);
    i2cStop(&sfm);

    
    delayMs(100);


    i2cStart(&sfm);
    s1  = i2cStartWrite(&sfm);
    s2  = i2cWrite(&sfm, START_MSB);
    s3  = i2cWrite(&sfm, START_LSB);

    sprintf(s, "Start: %d %d %d \n", s1, s2 ,s3);
    serialWriteString(s);

    delayMs(100);
    
    i2cStart(&sfm);
    s1  = i2cStartRead(&sfm);
    sprintf(s, "Start Read: %d\n",s1);
    short as = 0;
    for(i = 0; i < 3; i++){
        sprintf(s, "Start Read: %d %d %d\n",I2C1CON & 0x1F, I2C1STATbits.TRSTAT, i);
        serialWriteString(s);
        c1 = i2cRead(&sfm); 
        if(i < 2) i2cSendACK(&sfm);
        else i2cSendNACK(&sfm);
        
        sprintf(s, "ACK (%d): %d\n",i, c1);
        serialWriteString(s);
        if(i == 2) continue;
        as <<= 8; 
        as |= c1;
    }

    sprintf(s, "Dato: %hd\n", as);
    serialWriteString(s);
    i2cStop(&sfm);
/*
    i2cStart(&sfm);
    s1  = i2cWrite(&sfm, ADD << 1);
    s2  = i2cWrite(&sfm, START_MSB);
    s3  = i2cWrite(&sfm, START_LSB);
    a = 0;
    for(i = 0; i < 2; i++){
        a = (a | i2cRead(&sfm))<<8;
    }
    i2cStop(&sfm);
    sprintf(s, "Dato extraño: %d\n", a);
    serialWriteString(s);
    delayMs(10);

    i2cStart(&sfm);
    s1  = i2cWrite(&sfm, ADD << 1);
    s2  = i2cWrite(&sfm, START_MSB);
    s3  = i2cWrite(&sfm, START_LSB);
    a = 0;
    for(i = 0; i < 2; i++){
        a = (a | i2cRead(&sfm))<<8;
    }
    i2cStop(&sfm);
    sprintf(s, "Dato extraño: %d\n", a);
    serialWriteString(s);
    delayMs(10);*/


    delayMs(1000);
    unsigned char u, v;
    while(1){
        /*i2cStart(&sfm);
        s1 = i2cWrite(&sfm, (ADD << 1) | 1);
        //i2cRestart(&sfm);
        u = i2cRead(&sfm); i2cSendACK(&sfm);
        sprintf(s,"U: %u\n", u);
        serialWriteString(s);
        v = i2cRead(&sfm);i2cSendACK(&sfm);
        crc = ((short)i2cRead(&sfm));i2cSendNACK(&sfm);
        press = (u << 8) | v;
        i2cStop(&sfm);

        sprintf(s,"%d %u %u\n", press, u, v);
        serialWriteString(s);*/
        digitalWrite(LED, 1);
        delayMs(100);
        digitalWrite(LED, 0);
        delayMs(100);
    }
     
    return 0;
}
#endif  
