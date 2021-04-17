#include "sensorsTasks.h"
#include "mahony.h"
#include "task.h"
#include "utils.h"
#include "M8Q.h"
#include "kalman.h"

mpu9250 myIMU;

m8q myGPS;

filter filter_roll, filter_pitch, filter_yaw;

float   roll,       pitch,      yaw,
        raw_roll,   raw_pitch,  raw_yaw,
        ax,         ay,         az, 
        gx,         gy,         gz, 
        mx,         my,         mz, 
        x,          y,          z; 
        
float z_ant = 0;
float Kdfilt = 0.01;

bmp388 myBMP;
float altitude,offset_alt;

emaFilter ema_bmp;
mvAvgFilter mvAvg_bmp;
filter filter_z;

void accelTask(){   
    readAcc(&myIMU);
    ax = myIMU.ax, ay = myIMU.ay, az = myIMU.az; 
    
    setReg(ACC_X,(float)(ax));
    setReg(ACC_Y,(float)(ay));
    setReg(ACC_Z,(float)(az));

    if( calib_status & 1 ){
        cleanFiltAcc(&myIMU.fAccX); 
        cleanFiltAcc(&myIMU.fAccY); 
        cleanFiltAcc(&myIMU.fAccZ);
        calib_status ^= 1;
    }
}

void gyroTask(){

    readGyro(&myIMU);

    gx = myIMU.gx, gy = myIMU.gy, gz = myIMU.gz; 
    
    setReg(GYRO_X, gx);
    setReg(GYRO_Y, gy);
    setReg(GYRO_Z, gz);

    if( calib_status & 2  ){
        cleanFiltGyro(&myIMU.fGyroX); 
        cleanFiltGyro(&myIMU.fGyroY); 
        cleanFiltGyro(&myIMU.fGyroZ);
        calib_status ^= 2;
    }

}

void magTask(){
    readMag(&myIMU);
    mx = myIMU.mx;
    my = myIMU.my;
    mz = myIMU.mz;
}

void altitudeTask(){
    
    bmp388ReadAltitude(&myBMP);
    //z = computeMvAvgFilter( &mvAvg_bmp, myBMP.altitude );
    z = computeEmaFilter( &ema_bmp, myBMP.altitude);
    z = computeFilter( &filter_z, z );
    
    setReg(Z_VAL, z);
    
}

void heightTask(void *argument){
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_SET);
    uint32_t tim = TIME;
    while(TIME - tim < 10);
    HAL_GPIO_WritePin(TRIGGER_GPIO_Port, TRIGGER_Pin, GPIO_PIN_RESET);
}

void gpsTask(){

    int ret = readLatLon(&myGPS); 
    setReg(GPS_STATE, ret);
    
    if(ret == GPS_OK){
        if(getReg(START_GPS) <= 0)
            myGPS.off_x = myGPS.latitude, myGPS.off_y = myGPS.longitud;
        
        int x_lat = myGPS.latitude - myGPS.off_x;
        int y_lon = myGPS.longitud - myGPS.off_y;
        

        setReg(GPS_AVAILABLE, 1);
        setReg(GPS_X, 0.01*x_lat),
        setReg(GPS_Y, 0.01*y_lon),
        setReg(GPS_CNT, myGPS.cnt++);     
    }
}


void rpyTask(){
    
    float rpy[3];
    mahonyUpdate(gx*PI/360.0f, gy*PI/360.0f, gz*PI/360.0f, ax, ay, az, my, mx, mz);
    getMahonyEuler(rpy);
    raw_roll = rpy[0], raw_pitch = rpy[1], raw_yaw = rpy[2];
    
    /*roll += fmax(fmin(Kdfilt, (rpy[0] - roll)),-Kdfilt);
    pitch += fmax(fmin(Kdfilt, (rpy[1] - pitch)),-Kdfilt);
    yaw += fmax(fmin(Kdfilt,(rpy[2] - yaw)),-Kdfilt);*/

    raw_roll = computeFilter(&filter_roll, raw_roll);
    raw_pitch = computeFilter(&filter_pitch, raw_pitch);
    raw_yaw = computeFilter(&filter_yaw, raw_yaw);

    setReg(RAW_ROLL, raw_roll);
    setReg(RAW_PITCH, raw_pitch);
    setReg(RAW_YAW, raw_yaw);
    
    roll = raw_roll - getReg(ROLL_OFFSET);
    pitch = raw_pitch - getReg(PITCH_OFFSET);
    yaw = raw_yaw - getReg(YAW_OFFSET);

    setReg(ROLL_VAL, roll);
    setReg(PITCH_VAL, pitch);
    setReg(YAW_VAL, yaw);

    if( calib_status & 8  ){
        cleanFilter(&filter_roll);
        cleanFilter(&filter_pitch);
        cleanFilter(&filter_yaw);
        calib_status ^= 8;
    }
}

void xyzTask(){
    
    
    if(getReg(START_GPS) > 0){
            kalmanUpdateIMU(ax, ay, az, raw_roll, raw_pitch, raw_yaw);

            if(getReg(GPS_AVAILABLE) > 0)
                setReg(GPS_AVAILABLE, 0),
                kalmanUpdateGPS(getReg(GPS_X), getReg(GPS_Y), 0);
    }
    else{
        clearKalman();
    }

    getPosition(&x, &y, &z);
    
    z = distance;
    if( fabs(z-z_ant) > 0.5  ) z = z_ant;
    z_ant = z;

    setReg(X_VAL, x);
    setReg(Y_VAL, y);
    setReg(Z_VAL, z);
}

void initSensorsTasks(){
    
    initMpu(&myIMU);

    initFilter(&filter_roll, 4, k_1_10, v_1_10);
    initFilter(&filter_pitch, 4, k_1_10, v_1_10);
    initFilter(&filter_yaw, 4, k_1_10, v_1_10);

    setReg(ACC_SCALE,1);
    setReg(MAG_X_SCALE,1);
    setReg(MAG_Y_SCALE,1);
    setReg(MAG_Z_SCALE,1);

    #if PORT == GPS
        initM8Q(&myGPS);
        setKalmanTsImu(0.01);
        setKalmanTsGps(0.5);

        initMatGlobal();
    #endif
    
    calib_status = 0;

    //initBmp388(&myBMP, 10);  
    //initMvAvgFilter(&mvAvg_bmp, 25);

    initEmaFilter(&ema_bmp, 0.9, 0.1, 0.8);
    initFilter(&filter_z, 4, k_1_20, v_1_20);

    addTask(&gyroTask, 1000, 3);
    addTask(&accelTask, 1000, 3);
    addTask(&magTask, 100000, 2);
    addTask(&rpyTask, 2000, 2);
    //addTask(&altitudeTask,10000,2);
    addTask(&heightTask, 10000, 2);
    
    #if PORT == GPS
        addTask(&gpsTask, 500000, 3);
        addTask(&xyzTask, 10000, 3);
    #endif

}
