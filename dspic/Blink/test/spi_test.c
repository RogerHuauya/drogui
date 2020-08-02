//#define SPI_TEST
#ifdef SPI_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "BMP280.h"
#define LED PRTD, 8

char crc = 0;
char buff[50];
int s1, s2, s3;

int main(){

    initConfig();
    initSerial();
    initBmp280();
    
    __delay_ms(1000);
    pinMode(LED, OUTPUT);
    char dat[5];

    int32_t raw_press = 0, raw_temp = 0;
    double press, temp; 
    while(1){
        raw_press = bmpReadPressure();
        press = bmp280CompensatePressure(raw_press);
        raw_temp = bmpReadTemperature();
        temp = bmp280CompensateTemperature(raw_temp);
        
        char reg = 0x0;
        int i;
        /*
        sprintf(buff,"Raw data = %ld %ld\t", raw_press, raw_temp);
        serialWriteString(buff);*/
        sprintf(buff,"%.3lf %.3lf\n", press, temp);
        serialWriteString(buff);
        digitalToggle(LED);
        
        __delay_ms(100);
    }
    return 0;
}
#endif  
