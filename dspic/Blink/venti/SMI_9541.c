//#define SMI_9541
#ifdef SMI_9541

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"

#define LED PRTD, 8
#define ADD 0x28

int press = 0, temp;
char crc = 0;
char s[50];
int s1, s2, s3;
char c1,c2,c3;

int main(){

    initConfig();
    initSerial();
    initI2C();
    __delay_ms(1000);

    pinMode(LED, OUTPUT);


    while(1){
        
        i2cStart();
        s1 = i2cWrite((ADD << 1) | 1);
        sprintf(s,"%d\n", s1);
        serialWriteString(s);
        
        press  = ((int)i2cRead()<<8) & (0x3F); i2cSendACK();
        press |= ((int)i2cRead()); i2cSendACK();
        
        temp = ((int)i2cRead()<< 3); i2cSendACK();
        temp |= ((int)i2cRead() >> 5) & (7); i2cSendNACK();
        
        i2cStop();

        sprintf(s,"%d %d\n", press, temp);
        serialWriteString(s);

        digitalWrite(LED, 1);
        __delay_ms(100);
        digitalWrite(LED, 0);
        __delay_ms(100);
    }
    return 0;
}

#endif