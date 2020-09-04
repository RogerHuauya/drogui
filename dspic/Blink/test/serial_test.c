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



int main(){
    initConfig();
    initSerial();
    char s[50];

    while(1){
        serialWriteString("Hola mundo!!\n\r");
        __delay_ms(500);
    }
    return 0;
}



#endif