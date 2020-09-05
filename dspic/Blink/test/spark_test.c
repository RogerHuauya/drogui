#define SPARK_TEST
#ifdef SPARK_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"

#define ADD 0x08

char c1,c2,c3;
int main(){

    initConfig();
    initSerial();
    initI2C();
    __delay_ms(1000);

    while(1){
        
        
        i2cStart();

        i2cWrite((ADD << 1) | 1);
        int i = 0;
        char s[10];
        for(i = 0; i < 4; i++){
            s[i] = i2cRead();
        }
        s[4] = '\n';
        s[5] =  '\0';
        serialWriteString(s);
        s[0] =  '\0';
        __delay_ms(100);
    }
    return 0;
}
#endif  
