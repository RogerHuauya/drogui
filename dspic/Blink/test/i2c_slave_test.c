#define I2C_SLAVE_TEST
#ifdef I2C_SLAVE_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"

#define ADD 64
#define READ_CMD 0xF1


int press = 0;
char crc = 0;
char s[50];
int s1, s2, s3;


char c1,c2,c3;
i2c slave;
int main(){

    initConfig();
    initSerial();
    initI2C(&slave, I2C1, 0x55, 400000, SLAVE);
    __delay_ms(1000);

    while(1){
        serialWriteString("Nice\n");
    }
    return 0;
}
#endif  
