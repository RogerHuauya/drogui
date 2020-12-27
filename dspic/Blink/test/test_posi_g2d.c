//#define POSI_TEST
#ifdef POSI_TEST
#include "config.h"
#include "utils.h"
#include "i2c.h"
#include "serial.h"
#include "MM7150.h"
#include "timer.h"
#include "registerMap.h"
#include "kalman.h"

char buffer[50];
timer readSensors;
i2c slave;
serial Serial1;
sensor acc, ori;
double roll, pitch, yaw;
float x,y,z;

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

    if(getReg(START) > 0){
        kalmanUpdateIMU(acc.dDataX, acc.dDataY, acc.dDataZ, ori.dDataW, ori.dDataX, ori.dDataY, ori.dDataZ);

        if(getReg(GPS_AVAILABLE) == 1) setReg(GPS_AVAILABLE, 0), kalmanUpdateGPS(getReg(GPS_X), getReg(GPS_Y), 0);    
    }
    else{
        clearKalman();
    }

    getPosition(&x, &y, &z);
    
    setReg(X_VAL, x);
    setReg(Y_VAL, y);
    setReg(Z_VAL, z);
    
    ground2Drone( 1, 1, 1 );

    clearTimerFlag(&readSensors);
}


int main(){
    initConfig();
    initSerial(&Serial1, SERIAL1, 115200);
    
    initI2C(&slave, I2C2, 0x60, 400000, SLAVE);
    clearI2Cregisters(I2C2);
    
    initMM7150();    
    initAccel(&acc, 100, 1);
    initOrient(&ori, 50, 10);

    
    setKalmanTsImu(0.02);
    setKalmanTsGps(1);
    initMatGlobal();
    
    initTimer(&readSensors, 2, DIV256, 3);
    setTimerFrecuency(&readSensors, 100);

    while(1){
        sprintf(buffer, "Vx: %.3f\tVy: %.3f\tVz: %.3f\tX:%.3f\tY:%.3f\tZ:%.3f\n", getMatVal(&error_dpos, 0, 0),
                                                                    getMatVal(&error_dpos, 1, 0), getMatVal(&error_dpos, 2, 0));
        __delay_ms(20);
    }
    
    return 0;
}


#endif