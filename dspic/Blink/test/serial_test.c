//#define SERIAL_TEST
#ifdef SERIAL_TEST
#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"

serial Serial1;

int main(){
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    char s[50];

    while(1){
        serialWriteString(&Serial1,"Hola mundo!!\n\r");
        __delay_ms(500);
    }
    return 0;
}



#endif