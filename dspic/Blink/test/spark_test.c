//#define SPARK_TEST
#ifdef SPARK_TEST

#include "config.h"
#include "quaternionFilters.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"
#include "imu.h"
#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0
#define I2Cclock 100000

#define ADD 0x08
imu myIMU;
char c1,c2,c3;
int main(){
    char s[50];
    uint8_t c, d;
    int16_t accel[3], gyro[3], mag[3];
    initConfig();
    initSerial();
    serialWriteString("PIC initialized\n");
    initMPU9250(&myIMU, I2C1, I2Cclock);
    
    __delay_ms(1000);

    while(1){
        if (readByteIMU(&myIMU, MPU, INT_STATUS)& 0x01){
            readAll(&myIMU);
            //printIMU(&myIMU);
            MahonyQuaternionUpdate(myIMU.ax, myIMU.ay, myIMU.az, myIMU.gx * PI/180.0f,
                         myIMU.gy * PI/180.0f, myIMU.gz * PI/180.0f, myIMU.my,
                         myIMU.mx, myIMU.mz, 100);
            sprintf(s,"%.3f %.3f %.3f %.3f \n", *getQ(), *(getQ() + 1), *(getQ() + 2), *(getQ() + 3));
            serialWriteString(s);
        }
        else serialWriteString("test\n");
        __delay_ms(100);
    }
    return 0;
}
#endif  
