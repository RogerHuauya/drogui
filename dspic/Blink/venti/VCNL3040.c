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
#define ADD 0x60

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
        s1 = i2cWrite((ADD << 1));
        s2 = i2cWrite(0x0C);
        sprintf(s,"ACK Write: %d ID: %d\n", s1, s2);
        serialWriteString(s);
        
        i2cRestart();
        
        s1 = i2cWrite((ADD << 1) | 1);
        
        sprintf(s,"ACK Read: %d\n", s1);
        serialWriteString(s);

        press  = ((int)i2cRead()<<8); i2cSendACK();
        press |= ((int)i2cRead()); i2cSendNACK();
        i2cStop();

        sprintf(s,"Answer: %d \n", press);
        serialWriteString(s);

        digitalWrite(LED, 1);
        __delay_ms(100);
        digitalWrite(LED, 0);
        __delay_ms(100);
    }
    return 0;
}

#endif
