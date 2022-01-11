//#define KALMAN_TEST
#ifdef KALMAN_TEST

#include "_main.h"
#include "serial.h"
#include "SAM_M8Q.h"
#include "kalman.h"
#include "sensorsTasks.h"
#include "registerMap.h"
#include "debugTasks.h"
#include "task.h"
#include "utils.h"

m8q myGPS;
int cnt = 0;
int off_x, off_y;
//float x, y;


void gpsTask(){

    int ret = readLatLon(&myGPS);
    setReg(GPS_STATE, ret);

    if(ret == GPS_OK){
        if(getReg(START) <= 0)
            off_x = myGPS.latitude, off_y = myGPS.longitud;

        setReg(GPS_AVAILABLE, 1);
        setReg(GPS_X, myGPS.latitude - off_x),
        setReg(GPS_Y, myGPS.longitud - off_y),
        setReg(GPS_CNT,   cnt++);
    }
}



void kalmanTask(){


    if(getReg(START) > 0){
            kalmanUpdateIMU(ax, ay, az, raw_roll, raw_pitch, raw_yaw);

            if(getReg(GPS_AVAILABLE) > 0)
                setReg(GPS_AVAILABLE, 0),
                kalmanUpdateGPS(getReg(GPS_X)/100.0, getReg(GPS_Y)/100.0, 0);
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


    setKalmanTsImu(0.01);
    setKalmanTsGps(0.5);

    initMatGlobal();

    initDebug();
    initSensorsTasks();
    addTask(&gpsTask, 500000, 3);
    addTask(&kalmanTask, 10000, 3);

    initRTOS();
}


#endif
