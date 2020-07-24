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
#define LED PRTD, 4
#define ADD 64
#define READ_CMD 0xF1
int press = 0;
char crc = 0;
char s[50];
int s1, s2, s3;
int main(){
    initConfig();
    initSerial();
    initI2C();
    __delay_ms(1000);

    pinMode(LED, OUTPUT);


    while(1){
        
        i2cStart();
        serialWriteString("I2c start\n");
        s1 = i2cWrite(ADD << 1 );
        sprintf(s,"result write: %d\n",s1);
        serialWriteString(s);
        s2 = i2cWrite(READ_CMD);
        sprintf(s,"result command: %d\n",s2);
        serialWriteString(s);
        
        s1 = i2cWrite((ADD << 1) | 1);
        sprintf(s,"result: %d\n",s1);
        serialWriteString(s);

        press  = ((int)i2cRead()<<8); i2cSendACK();
        press |= ((int)i2cRead()); i2cSendACK();
        crc = ((short)i2cRead()); i2cSendACK();
        i2cStop();

        sprintf(s,"Presion %d Checksum %d\n", press, crc);
        serialWriteString(s);
        digitalWrite(LED, 1);
        __delay_ms(100);
        digitalWrite(LED, 0);
        __delay_ms(100);
    }
    return 0;
}
#endif  
