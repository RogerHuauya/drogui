//#define VCNL3040
#ifdef VCNL3040

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"

#define LED PRTD, 8
#define INT_READ PRTD, 9
#define ADD 0x60
#define CONFIG_12 0x03
#define CONFIG_3MS 0x04
#define CONFIG_CANC 0x05
#define THDL 0x06
#define THDH 0x07
#define DATA 0x08



int press = 0, temp;
char crc = 0;
char s[50];
int s1, s2, s3;
unsigned char c1,c2,c3;


int main(){

    initConfig();
    initSerial();
    initI2C();
    __delay_ms(1000);

    pinMode(LED, OUTPUT);
    
    i2cStart();
    i2cWrite((ADD << 1));
    s1 = i2cWrite(CONFIG_12);
    s2 = i2cWrite(0xFC);
    s3 = i2cWrite(0x03);

    sprintf(s,"Command %d PS_CONFIG1: %d PS_CONFIG2: %d\n", s1, s2, s3);
    serialWriteString(s);
    i2cStop();

    i2cStart();
    i2cWrite((ADD << 1));
    s1 = i2cWrite(CONFIG_3MS);
    s2 = i2cWrite(0x51);
    s3 = i2cWrite(0x25);

    sprintf(s,"Command %d PS_CONFIG3: %d PS_CONFIGMS: %d\n", s1, s2, s3);
    serialWriteString(s);
    i2cStop();

    i2cStart();
    i2cWrite((ADD << 1));
    s1 = i2cWrite(THDL);
    s2 = i2cWrite(0x00);
    s3 = i2cWrite(0x0F);

    sprintf(s,"Command %d THDLL: %d THDLH: %d\n", s1, s2, s3);
    serialWriteString(s);
    i2cStop();


    i2cStart();
    i2cWrite((ADD << 1));
    s1 = i2cWrite(THDH);
    s2 = i2cWrite(0x00);
    s3 = i2cWrite(0xF0);

    sprintf(s,"Command %d THDHL: %d THDHH: %d\n", s1, s2, s3);
    serialWriteString(s);
    i2cStop();


    while(1){
        
        i2cStart();
        s1 = i2cWrite((ADD << 1));
        s2 = i2cWrite(DATA);
        //sprintf(s,"ACK Write: %d Data: %d\n", s1, s2);
        //serialWriteString(s);
        
        i2cStart();
        
        s1 = i2cWrite((ADD << 1) | 1);
        
        //sprintf(s,"ACK Read: %d\n", s1);
        //serialWriteString(s);

        /*press  = ((int)i2cRead()<<8); i2cSendACK();
        press |= ((int)i2cRead()); i2cSendNACK();*/

        c1 = i2cRead(); i2cSendACK();
        c2 = i2cRead(); i2cSendNACK();
        i2cStop();
	int dis = ( ((int)c2) << 8 ) + ((int) c1);
        sprintf(s,"%d\n", dis);
        serialWriteString(s);

        digitalWrite(LED, 1);
        __delay_ms(100);
        digitalWrite(LED, 0);
        __delay_ms(100);
    }
    return 0;
}

#endif
