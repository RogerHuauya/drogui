//#define SPARK_TEST
#ifdef SPARK_TEST

#include "config.h"
#include <libpic30.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "serial.h"
#include "i2c.h"
#include "imu.h"
#include "MahonyAHRS.h"
#include "utils.h"

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
            /*printIMU(&myIMU);
            break;*/
            MahonyAHRSupdate( myIMU.gx * pi/180.0f,myIMU.gy * pi/180.0f, myIMU.gz * pi/180.0f,  
                                    myIMU.ax, myIMU.ay, myIMU.az, 
                                    myIMU.mx, myIMU.my, myIMU.mz);

            sprintf(s,"%.3lf\t%.3lf\t%.3lf\n",
                *(getMahonyEuler()), *(getMahonyEuler()+1), *(getMahonyEuler()+2));
            serialWriteString(s);
        }
        else serialWriteString("test\n");
        __delay_ms(2);
    }
    return 0;
}

#endif  
