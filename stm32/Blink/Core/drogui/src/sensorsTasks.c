#include "sensorsTasks.h"
#include "_freertos.h"


mpu9250 myIMU;

filter filter_gx, filter_gy, filter_gz;
filter filter_gx2, filter_gy2, filter_gz2;
filter filter_ax, filter_ay, filter_az;

float roll, pitch, yaw, ax, ay, az, gx, gy, gz, mx, my, mz, x, y, z;

void accelTask(){
    for(;;){
        readAcc(&myIMU);
        
        ax = computeFilter(&filter_ax, myIMU.ax);
        ay = computeFilter(&filter_ay, myIMU.ay);
        az = computeFilter(&filter_az, myIMU.az);
        

        setReg(ACC_X,(float)(ax));
        setReg(ACC_Y,(float)(ay));
        setReg(ACC_Z,(float)(az));
        osDelay(1);
    }
}

void gyroTask(){
    for(;;){
        readGyro(&myIMU);

        gx = computeFilter(&filter_gx, myIMU.gx);
        gy = computeFilter(&filter_gy, myIMU.gy);
        gz = computeFilter(&filter_gz, myIMU.gz);

        
        gx = computeFilter(&filter_gx2, gx);
        gy = computeFilter(&filter_gy2, gy);
        gz = computeFilter(&filter_gz2, gz);
        
        gx /= 5;
        gy /= 5;
        gz /= 5; 


        setReg(GYRO_X, gx);
        setReg(GYRO_Y, gy);
        setReg(GYRO_Z, gz);
        osDelay(1);
    }
}

void magTask(){
    for(;;){
        readMag(&myIMU);
        mx = myIMU.mx;
        my = myIMU.my;
        mz = myIMU.mz;
        osDelay(100);
    }
}





osThreadId_t accelHandle;
const osThreadAttr_t accelAttributes = {    .name = "accelTask", \
                                            .stack_size = 128 * 4, \
                                            .priority = (osPriority_t) osPriorityHigh1};


osThreadId_t gyroHandle;
const osThreadAttr_t gyroAttributes = {    .name = "gyroTask",\
                                            .stack_size = 128 * 4,\
                                            .priority = (osPriority_t) osPriorityHigh1};


osThreadId_t magHandle;
const osThreadAttr_t magAttributes = {    .name = "magTask",\
                                            .stack_size = 128 * 4,\
                                            .priority = (osPriority_t) osPriorityHigh1};



void initSensorsTasks(){
    
    initMpu(&myIMU);


    initFilter(&filter_gx, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_gy, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_gz, sz_1_10 , k_1_10, v_1_10);

    initFilter(&filter_ax, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_ay, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_az, sz_1_10 , k_1_10, v_1_10);

    initFilter(&filter_gx2, sz_3_10 , k_3_10, v_3_10);
    initFilter(&filter_gy2, sz_3_10 , k_3_10, v_3_10);
    initFilter(&filter_gz2, sz_3_10 , k_3_10, v_3_10);


    calibrateGyro(&myIMU);
    //calibrateAccel(&myIMU);
    //calibrateMag(&myIMU);
    
    accelHandle = osThreadNew(accelTask, NULL, &accelAttributes);
    gyroHandle = osThreadNew(gyroTask, NULL, &gyroAttributes);
    magHandle = osThreadNew(magTask, NULL, &magAttributes);
    /*initTimer(&timer_accel, &accelInterrupt, 1000);
    initTimer(&timer_gyro, &gyroInterrupt, 1000);
    initTimer(&timer_mag, &magInterrupt, 10);
    initTimer(&timer_rpy, &rpyInterrupt, 500);*/
}