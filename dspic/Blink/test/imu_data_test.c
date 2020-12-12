#define IMU_DATA_TEST
#ifdef IMU_DATA_TEST

#include <xc.h>
#include "config.h"
#include "MM7150.h"
#include "serial.h"
#include <math.h>
#include "i2c.h"
#include "timer.h"
#include "registerMap.h"
#include "utils.h"

serial Serial1;
sensor acc, gyro, ori, inc;
double roll, pitch, yaw;
timer readSensors;
char buffer[100];
i2c slave;
float Ts = 0.01;
int cont2 = 0;

void timerInterrupt(2){
    readOrient(&ori);        
    readAccel(&acc);

    getEuler(ori.dDataW, ori.dDataX, ori.dDataY, ori.dDataZ, &roll, &pitch, &yaw);
    
    setReg(ACC_X, acc.dDataX);
    setReg(ACC_Y, acc.dDataY);
    setReg(ACC_Z, acc.dDataZ);
    
    setReg(ROLL_VAL, roll);
    setReg(PITCH_VAL, pitch);
    setReg(YAW_VAL, yaw);
    

    clearTimerFlag(&readSensors);
}

int main(){
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    clearI2Cregisters(I2C2);
    
    char s[50];
    initMM7150();
    initAccel(&acc, 100, 1);
    initOrient(&ori, 50, 10);

    initTimer(&readSensors, 2, DIV256, 3);
    setTimerFrecuency(&readSensors, 100);


    while(1){
        /*if(cont2 >= 100){
            cont2 = 0;
            serialWriteString(&Serial1, "hola\n");
        }*/
        __delay_ms(1);
    }
    return 0;
}

#endif