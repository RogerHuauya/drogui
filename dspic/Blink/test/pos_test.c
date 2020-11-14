#define POS_TEST_H
#ifdef POS_TEST_H

#include <xc.h>
#include "config.h"
#include "MM7150.h"
#include "serial.h"
#include <math.h>
#include "MahonyAHRS.h"
#include "i2c.h"
#include "timer.h"
#include "registerMap.h"
#include "utils.h"

#define PI 3.14159264
i2c slave;
serial Serial1;
sensor acc, ori;
double roll, pitch, yaw;
timer readSensors;
char buffer[50];
void getEuler(double q0,double q1,double q2, double q3);
double angle_dif(double angle1, double angle2){
    if(angle1 > angle2){
        if((angle1 - angle2) > (2*pi - angle1 + angle2)) return -2*pi + angle1 - angle2;
        else return angle1 - angle2;
    }
    else{
        if((angle2 - angle1) > (2*pi - angle2 + angle1)) return 2*pi - angle2 + angle1;
        else return angle1 - angle2;
    }
}
float dt = 0.01, v[3] = {0, 0, 0}, x[3] = {0, 0, 0};
/*
void timerInterrupt(2){
    //readOrient(&ori);        
    readAccel(&acc);
    //getEuler(ori.dDataW, ori.dDataX, ori.dDataY, ori.dDataZ);
    v[0] += acc.dDataX*dt;
    v[1] += acc.dDataY*dt;
    v[2] += acc.dDataZ*dt;

    x[0] += v[0];
    x[1] += v[1];
    x[2] += v[2];

    
    /*
    setReg(ROLL_DEG,(float)(roll));
    setReg(PITCH_DEG,(float)(pitch));
    setReg(YAW_DEG,(float)(yaw));
    */
    //clearTimerFlag(&readSensors);
//}*/

int main(){
    initConfig();

    //setTimerFrecuency(&readSensors, 100);

    initSerial(&Serial1, SERIAL1, 115200);
    char s[50];
    initMM7150();
    initAccel(&acc, 100, 20);
    initOrient(&ori, 100,20);
    double q0, q1, q2, q3, roll, pitch, yaw; 
    //initTimer(&readSensors, 2, DIV256, 3);
    while(1){
        readAccel(&acc);
        //getEuler(ori.dDataW, ori.dDataX, ori.dDataY, ori.dDataZ);
        v[0] += acc.dDataX*dt;
        v[1] += acc.dDataY*dt;
        v[2] += acc.dDataZ*dt;

        x[0] += v[0];
        x[1] += v[1];
        x[2] += v[2];

        sprintf(buffer, "%.3f %.3f %.3f %.3f %.3f %.3f\n", acc.dDataX, acc.dDataY, acc.dDataZ, x[0], x[1], x[2]);
        serialWriteString(&Serial1, buffer);
        __delay_ms(100);
    }
    return 0;
}

void getEuler(double q0, double q1, double q2, double q3){
	
    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q0 * q1 + q2 * q3);
    double cosr_cosp = 1 - 2 * (q1 * q1 + q2 * q2);
    roll = atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (q0 * q2 - q3 * q1);
    if (abs(sinp) >= 1)
        pitch = copysign(pi / 2, sinp); // use 90 degrees if out of range
    else
        pitch = asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q0 * q3 + q1 * q2);
    double cosy_cosp = 1 - 2 * (q2 * q2 + q3 * q3);
    yaw = atan2(siny_cosp, cosy_cosp);
}
#endif