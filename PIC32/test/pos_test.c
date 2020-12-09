#define POS_TEST
#ifdef POS_TEST

#include <xc.h>
#include "config.h"
//#include "MM7150.h"
#include "serial.h"
#include <math.h>
//#include "MahonyAHRS.h"
#include "i2c.h"
#include "timer.h"
//#include "registerMap.h"
#include "utils.h"
#include "matlib.h"
#include "kalman.h"

#define G 0.0981

//sensor acc, gyro, ori, inc;
double roll, pitch, yaw;
timer readSensors;
char buffer[100];
i2c slave;
float Ts = 0.01;

extern mat bias_u;

void timerInterrupt(2, 4){
       
    quaternionToR(&Rq, 0.006, 0.077, 0.996, -0.007);

    setMatVal(&s, 0, 0, 0.004*G);
    setMatVal(&s, 1, 0, 0.009*G);
    setMatVal(&s, 2, 0, 1.032*G);
    
    //serialWriteString("pio\n");
    kalmanUpdate();

    clearTimerFlag(&readSensors);
}

int main(){
    initPBCLK();
      
    //initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    //clearI2Cregisters(I2C2);
    initMatGlobal();

    initSerial(115200, 4);
    serialWriteString("Init\n");
    delayMs(3000);
    char s[50];
    /*
    initMM7150();
    initAccel(&acc, 100, 20);
    initOrient(&ori, 50, 10);
    */
    initTimer(&readSensors, 2, 2, DIV256);
    setTimerFrecuency(&readSensors,  20);


    while(1){
        
        /*sprintf(buffer, "Vx: %.3f\tVy: %.3f\tVz: %.3f\tX:%.3f\tY:%.3f\tZ:%.3f\n", getMatVal(&v, 0, 0),
                                                                    getMatVal(&v, 1, 0), getMatVal(&v, 2, 0),
                                                                    getMatVal(&p, 0, 0),
                                                                    getMatVal(&p, 1, 0), 
                                                                    getMatVal(&p, 2, 0));
        
        serialWriteString(&Serial1, buffer);*/
        //serialWriteString("pollo\n");
        delayMs(20);
    }
    return 0;
}

#endif