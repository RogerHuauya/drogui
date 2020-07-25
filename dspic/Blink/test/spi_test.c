#define SPI_TEST
#ifdef SPI_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "spi.h"
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
    initSPI();
    __delay_ms(1000);
    pinMode(LED, OUTPUT);
    uint8_t data[3];
    int32_t press = 0;
    while(1){
        
        spiWriteByte(0xF7);
        spiReadBlock(data, 3);
        press = (data[0]<<8  | data[1]) << 4 | data[3];
        sprintf(s,"Data pressure = %ld\n", press);
        serialWriteString(s);
        __delay_ms(10);
    }
    return 0;
}
#endif  
