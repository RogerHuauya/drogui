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
#include "kalman.h"

#define G 9.81

serial Serial1;
sensor acc, gyro, ori, inc;
double roll, pitch, yaw;
timer readSensors;
char buffer[100];
i2c slave;
float Ts = 0.01;

void timerInterrupt(2){
    readOrient(&ori);        
    readAccel(&acc);
    
    quaternionToR(&Rq, ori.dDataW, ori.dDataX, ori.dDataY, ori.dDataZ);

    setMatVal(&u, 0, 0, acc.dDataX*G);
    setMatVal(&u, 1, 0, acc.dDataY*G);
    setMatVal(&u, 2, 0, acc.dDataZ*G);
    kalmanUpdate();

    
    clearTimerFlag(&readSensors);
}

int main(){
    initConfig();
      
    initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    clearI2Cregisters(I2C2);
    initMatGlobal();

    initSerial(&Serial1, SERIAL1, 115200);
    char s[50];
    initMM7150();
    initAccel(&acc, 100, 20);
    initOrient(&ori, 50, 10);

    setTimerFrecuency(&readSensors, 100);
    initTimer(&readSensors, 2, DIV256, 3);

    while(1){
        
        sprintf(buffer, "Vx: %.3f\tVy: %.3f\tVz: %.3f\tX:%.3f\tY:%.3f\tZ:%.3f\n", getMatVal(&v, 0, 0),
                                                                    getMatVal(&v, 1, 0), getMatVal(&v, 2, 0),
                                                                    getMatVal(&p, 0, 0),
                                                                    getMatVal(&p, 1, 0), 
                                                                    getMatVal(&p, 2, 0));
        //serialWriteString(&Serial1, buffer);
        __delay_ms(20);
    }
    return 0;
}

#endif