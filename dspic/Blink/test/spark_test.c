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
#define I2Cclock 100000

#define ADD 0x08
mpu9250 mpu;
char c1,c2,c3;
int main(){
    char s[50];
    uint8_t c, d;
    int16_t accel[3], gyro[3], mag[3];
    initConfig();
    initSerial();
    initMPU9250(&mpu, I2C1, I2Cclock);
    __delay_ms(1000);

    while(1){
        if (readByteWire(&(mpu.mpuI2C), INT_STATUS)& 0x01){
            readAll(&mpu);
            printIMU(&mpu);
        }
        else serialWriteString("perra\n");
        __delay_ms(500);
    }
    return 0;
}
#endif  
