//#define MM7150_TEST
#ifdef MM7150_TEST
#include "config.h"
#include "MM7150.h"
#include "serial.h"
#include <math.h>
#include "MahonyAHRS.h"
#include "i2c.h"
#define PI 3.14159264
i2c slave;
serial Serial1;
sensor acc, ori;
int main(){
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    char s[50];
    initMM7150();
    initAccel(&acc, 100, 20);
    initOrient(&ori, 100,20);
    double q0, q1, q2, q3, roll, pitch, yaw; 
    while(1){
        readOrient(&ori);
        q0 = ori.dDataW;
        q1 = ori.dDataX;
        q2 = ori.dDataY;
        q3 = ori.dDataZ;
        //sprintf(s, "%.3f %.3f %.3f %.3f\n", ori.dDataX, ori.dDataY, ori.dDataZ, ori.dDataW);
        //serialWriteString(&Serial1, s);
            // roll (x-axis rotation)
        double sinr_cosp = 2 * (q0 * q1 + q2 * q3);
        double cosr_cosp = 1 - 2 * (q1 * q1 + q2 * q2);
        roll = atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        double sinp = 2 * (q0 * q2 - q3 * q1);
        if (abs(sinp) >= 1)
            pitch= copysign(PI / 2, sinp); // use 90 degrees if out of range
        else
            pitch = asin(sinp);

        // yaw (z-axis rotation)
        double siny_cosp = 2 * (q0 * q3 + q1 * q2);
        double cosy_cosp = 1 - 2 * (q2 * q2 + q3 * q3);
        yaw = atan2(siny_cosp, cosy_cosp);
        sprintf(s, "%.3f %.3f %.3f\n", roll, pitch, yaw);
        serialWriteString(&Serial1, s);
        __delay_ms(10);
    }
    return 0;
}

#endif