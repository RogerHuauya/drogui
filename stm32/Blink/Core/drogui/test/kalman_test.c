#define KALMAN_TEST
#ifdef KALMAN_TEST

#include "_main.h"
#include "serial.h"
#include "M8Q.h"
#include "kalman.h"
#include "sensorsTasks.h"
#include "registerMap.h"
#include "debugTasks.h"
#include "task.h"
#include "utils.h"

m8q myGPS;
int flag = 0, head = 0, cnt = 0;
int ret, pos_x, pos_y;
//float x, y;


void gpsTask(){

    ret = readLatLon(&myGPS); 
    setReg(GPS_STATE, ret);
    
    if(ret == GPS_OK){
        if(getReg(START) <= 0)
            pos_x = myGPS.latitude, pos_y = myGPS.longitud;
        
        setReg(GPS_X, myGPS.latitude - pos_x),
        setReg(GPS_Y, myGPS.longitud - pos_y),
        setReg(GPS_CNT,   cnt++);     
    }
}



void kalmanTask(){

    
    if(getReg(START) > 0){
        if(ret == GPS_OK){
            kalmanUpdateIMU(ax, ay, az, roll, pitch, yaw);
            kalmanUpdateGPS(getReg(GPS_X)/100.0, getReg(GPS_Y)/100.0, 0);    
        }
    }
    else{
        clearKalman();
    }

    getPosition(&x, &y, &z);
    
    setReg(X_VAL, x);
    setReg(Y_VAL, y);
    setReg(Z_VAL, z);

}

void _main(){
    
    HAL_Delay(1000);
    cfgM8QPort(&(myGPS.snd_pack), defaultCfgPort);
    
    HAL_Delay(100);
    changeBaudrate(460800);
    HAL_Delay(100);

    cfgM8QRate(&(myGPS.snd_pack), defaultCfgRate);
    readM8Q(&(myGPS.rcv_pack), 10000);

    cfgM8QMsg(&(myGPS.snd_pack), defaultCfgMsg); 
    readM8Q(&(myGPS.rcv_pack), 10000);

    HAL_Delay(100);
    
    
    setKalmanTsImu(0.001);
    setKalmanTsGps(0.5);

    initMatGlobal();

    initDebug();
    initSensorsTasks();
    addTask(&gpsTask, 500000, 3);
    addTask(&kalmanTask, 1000, 3);
    
    initRTOS();
}


#endif