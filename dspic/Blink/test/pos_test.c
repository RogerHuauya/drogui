#define POS_TEST
#ifdef POS_TEST

#include <xc.h>
#include <dsp.h>
#include "config.h"
#include "MM7150.h"
#include "serial.h"
#include <math.h>
#include "MahonyAHRS.h"
#include "i2c.h"
#include "timer.h"
#include "registerMap.h"
#include "utils.h"
#include "matlib.h"


#define PI 3.14159264

fractional x;
i2c slave;
serial Serial1;
sensor acc, gyro, ori, inc;
double roll, pitch, yaw;
timer readSensors;
char buffer[50];

float dt = 0.01, v[3] = {0, 0, 0}, x[3] = {0, 0, 0};
mat Rq, s, acel;

void timerInterrupt(2){
    readOrient(&ori);        
    readAccel(&acc);
    
    quaternionToR(&Rq, ori.dDataW, ori.dDataX, ori.dDataY, ori.dDataZ);
    
    setMatVal(s.val, 0, 0, acc.dDataX);
    setMatVal(s.val, 1, 0, acc.dDataY);
    setMatVal(s.val, 2, 0, acc.dDataZ);

    matMult(&acel, &Rq, &s); 
    
    v[0] += acel.val[0][0]*dt;
    v[1] += acel.val[1][0]*dt;
    v[2] += acel.val[2][0]*dt;

    x[0] += v[0];
    x[1] += v[1];
    x[2] += v[2];
    
   
    clearTimerFlag(&readSensors);
}

int main(){
    initConfig();
    matInit(&acel, 3, 1);
    matInit(&s, 3, 1);
    
    initSerial(&Serial1, SERIAL1, 115200);
    char s[50];
    initMM7150();
    initAccel(&acc, 100, 20);
    initOrient(&ori, 50, 200);

    setTimerFrecuency(&readSensors, 100);
    initTimer(&readSensors, 2, DIV256, 3);

    while(1){
        sprintf(buffer, "X:%.3f\tY:%.3f\tZ:%.3f\n", x[0], x[1], x[2]);
        serialWriteString(&Serial1, buffer);
        __delay_ms(20);
    }
    return 0;
}

#endif