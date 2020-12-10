//#define VCNL3040
#ifdef VCNL3040

#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"

#define LED PRTD, 9
#define INT_READ PRTD, 8
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

i2c vnc;

int main(){

    initPBCLK();
    initSerial(115200, 4);
    initI2C(&vnc, I2C1, ADD, 100000);
    delayMs(1000);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, 1);
    sprintf(s, "System initialized\n");
    serialWriteString(s);
    i2cStart(&vnc);
    i2cWrite(&vnc,(ADD << 1));
    s1 = i2cWrite(&vnc, CONFIG_12);
    s2 = i2cWrite(&vnc, 0x00);
    s3 = i2cWrite(&vnc, 0x00);
    i2cStop(&vnc);

    delayMs(100);
    sprintf(s,"Command %d PS_CONFIG1: %d PS_CONFIG2: %d\n", s1, s2, s3);
    serialWriteString(s);
    serialWriteString("1\n");

    i2cStart(&vnc);
    i2cWrite(&vnc, (ADD << 1));
    serialWriteString("2\n");
    s1 = i2cWrite(&vnc, CONFIG_3MS);
    serialWriteString("3\n");
    s2 = i2cWrite(&vnc, 0x51);
    serialWriteString("4\n");
    s3 = i2cWrite(&vnc, 0x25);
    serialWriteString("5\n");
    i2cStop(&vnc);

    sprintf(s,"Command %d PS_CONFIG3: %d PS_CONFIGMS: %d\n", s1, s2, s3);
    serialWriteString(s);

    i2cStart(&vnc);
    i2cWrite(&vnc, (ADD << 1));
    s1 = i2cWrite(&vnc, THDL);
    s2 = i2cWrite(&vnc, 0x00);
    s3 = i2cWrite(&vnc, 0x0F);

    sprintf(s,"Command %d THDLL: %d THDLH: %d\n", s1, s2, s3);
    serialWriteString(s);
    i2cStop(&vnc);


    i2cStart(&vnc);
    i2cWrite(&vnc, (ADD << 1));
    s1 = i2cWrite(&vnc, THDH);
    s2 = i2cWrite(&vnc, 0x00);
    s3 = i2cWrite(&vnc, 0xF0);

    sprintf(s,"Command %d THDHL: %d THDHH: %d\n", s1, s2, s3);
    serialWriteString(s);
    i2cStop(&vnc);


    while(1){
        
        i2cStart(&vnc);
        s1 = i2cWrite(&vnc, (ADD << 1));
        s2 = i2cWrite(&vnc, DATA);
        //sprintf(s,"ACK Write: %d Data: %d\n", s1, s2);
        //serialWriteString(s);
        
        i2cStart(&vnc);
        
        s1 = i2cWrite(&vnc,(ADD << 1) | 1);
        
        //sprintf(s,"ACK Read: %d\n", s1);
        //serialWriteString(s);

        /*press  = ((int)i2cRead()<<8); i2cSendACK();
        press |= ((int)i2cRead()); i2cSendNACK();*/

        c1 = i2cRead(&vnc); i2cSendACK(&vnc);
        c2 = i2cRead(&vnc); i2cSendNACK(&vnc);
        i2cStop(&vnc);
	int dis = ( ((int)c2) << 8 ) + ((int) c1);
        sprintf(s,"%d\n", dis);
        serialWriteString(s);

        digitalWrite(LED, 1);
        delayMs(100);
        digitalWrite(LED, 0);
        delayMs(100);
    }
    return 0;
}
#endif
