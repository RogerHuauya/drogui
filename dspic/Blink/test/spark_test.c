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
    if (c == 0x71)  serialWriteString("MPU9250 is online..."); // WHO_AM_I should always be 0x71
    else sprintf(s, "Id: %hhu\n", c), serialWriteString(s);
    MPU9250SelfTest(&mpu);
    serialWriteString("alv\n");
    for(int i = 0; i < 6; i++){
        sprintf(s,"%.3f ", mpu.selfTest[i]);
        serialWriteString(s);
    }
    serialWriteChar('\n');
    calibrateMPU9250(&mpu);
    serialWriteString("Calibration finished\n");
    mpu.mpuI2C.address = AK8963_ADDRESS;
    c = readByteWire(&mpu, WHO_AM_I_AK8963);
    if (c != 0x48){
        sprintf(s, "Id: %hhu\n", c);
        serialWriteString(s);
        serialWriteString("Communication failed, abort!\n");
    }
    else serialWriteString("asserted\n");
    serialWriteString("MPU9250 initialized for active data mode....\n");
    awakeMPU9250(&mpu);
      // Communication failed, stop here

    __delay_ms(1000);
    serialWriteString("alv2\n");
    while(1){
        if (readByteWire(&mpu, INT_STATUS) & 0x01){
            serialWriteString("Reading ...\n");
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
