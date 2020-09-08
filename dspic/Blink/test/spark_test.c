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
#include "mpu9250.h"
#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0
#define I2Cclock 400000

#define ADD 0x08
mpu9250 mpu;
char c1,c2,c3;
int main(){
    char s[50];
    uint8_t c;
    int16_t accel[3];
    initConfig();
    initSerial();
    initMPU9250(&mpu, MPU9250_ADDRESS, I2Cclock);
    __delay_ms(1000);
    c = readByteWire(&mpu, WHO_AM_I_MPU9250);
    sprintf(s, "Id: %hhu\n", c);
    if (c == 0x71) // WHO_AM_I should always be 0x71
        serialWriteString("MPU9250 is online...");
    else serialWriteString(s);
    serialWriteString("alv\n");
    float dest[50];
    MPU9250SelfTest(&mpu, &dest);
    calibrateMPU9250(&mpu);
    awakeMPU9250(&mpu);
    __delay_ms(1000);
    serialWriteString("alv2\n");
    while(1){
        if (readByteWire(&mpu, INT_STATUS) & 0x01){
            readAccelData(&mpu, accel);
            sprintf(s, "%d %d %d\n",accel[0], accel[1], accel[2]);
            serialWriteString(s);
        }
        else serialWriteString("perra\n");
        __delay_ms(500);
    }
    return 0;
}
#endif  
