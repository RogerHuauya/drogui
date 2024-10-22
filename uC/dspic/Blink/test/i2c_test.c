//#define I2C_TEST

/*#ifdef I2C_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"

#define LED PRTD, 8
#define ADD 64
#define READ_CMD 0xF1


int press = 0;
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
        i2cWrite(ADD << 1);
        i2cWrite(READ_CMD);
        
        i2cStop();
        
        i2cStart();

        i2cWrite((ADD << 1) | 1);
        
        press  = ((int)i2cRead()<<8); i2cSendACK();
        press |= ((int)i2cRead()); i2cSendACK();
        crc = ((short)i2cRead()); i2cSendACK();
        i2cStop();

        sprintf(s,"%.5lf\n", press/60.0);
        serialWriteString(s);
        digitalWrite(LED, 1);
        __delay_ms(10);
        digitalWrite(LED, 0);
        __delay_ms(10);
    }
    return 0;
}
#endif  
*/