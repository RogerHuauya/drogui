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

serial Serial1;
char crc = 0;
char s[50];

int main(){

    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    initBmp280();
    
    __delay_ms(1000);
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
        sprintf(s,"%.3lf %.3lf\n", press, temp);
        serialWriteString(&Serial1, s);
        
        __delay_ms(200);
    }
    return 0;
}
#endif  
