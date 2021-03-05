#include "sensorsTasks.h"
#include "mahony.h"
#include "task.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>


mpu9250 myIMU;

filter filter_gx, filter_gy, filter_gz;
filter filter_gx2, filter_gy2, filter_gz2;
dNotchFilter dnotch_gx, dnotch_gy, dnotch_gz; 
dNotchFilter dnotch_gx2, dnotch_gy2, dnotch_gz2;


filter filter_ax, filter_ay, filter_az;
dNotchFilter dnotch_ax, dnotch_ay, dnotch_az; 

filter filter_roll, filter_pitch, filter_yaw;
float roll, pitch, yaw, ax, ay, az, gx, gy, gz, mx, my, mz, x, y, z;


void accelTask(){   
    readAcc(&myIMU);

    ax = computeFilter(&filter_ax, myIMU.ax);
    ay = computeFilter(&filter_ay, myIMU.ay);
    az = computeFilter(&filter_az, myIMU.az);


    /*ax = computeDNotch(&dnotch_ax, ax);
    ay = computeDNotch(&dnotch_ay, ay);
    az = computeDNotch(&dnotch_az, az);*/

    setReg(ACC_X,(float)(ax));
    setReg(ACC_Y,(float)(ay));
    setReg(ACC_Z,(float)(az));
}

char buff2[50] = "hola\n";
void gyroTask(){
    //sprintf(buff2, "%lu;\n",TIME);
    //HAL_UART_Transmit(&huart2, (uint8_t*) buff2, strlen(buff2), 100);
    readGyro(&myIMU);

    gx = computeFilter(&filter_gx, myIMU.gx);


    /*
    sprintf(buff2, "%f\t%f ;\n", myIMU.gx, gx);
    HAL_UART_Transmit(&huart2, (uint8_t*) buff2, strlen(buff2), 100);*/
    gy = computeFilter(&filter_gy, myIMU.gy);
    gz = computeFilter(&filter_gz, myIMU.gz);

    
    
    gx = computeFilter(&filter_gx2, gx);
    gy = computeFilter(&filter_gy2, gy);
    gz = computeFilter(&filter_gz2, gz);

    gx = computeDNotch(&dnotch_gx, gx);
    gy = computeDNotch(&dnotch_gy, gy);
    gz = computeDNotch(&dnotch_gz, gz);

    gx = computeDNotch(&dnotch_gx2, gx);
    gy = computeDNotch(&dnotch_gy2, gy);
    gz = computeDNotch(&dnotch_gz2, gz);
    
    gx /= 5;
    gy /= 5;
    gz /= 5; 


    setReg(GYRO_X, gx);
    setReg(GYRO_Y, gy);
    setReg(GYRO_Z, gz);
}

void magTask(){
    readMag(&myIMU);
    mx = myIMU.mx;
    my = myIMU.my;
    mz = myIMU.mz;
}

float Kdfilt = 0.0005;
void rpyTask(){
    
    float rpy[3];
    mahonyUpdate(gx*PI/360.0f, gy*PI/360.0f, gz*PI/360.0f, ax, ay, az, my, mx, mz);
    getMahonyEuler(rpy);
    //roll = rpy[0], pitch = rpy[1], yaw = rpy[2];
    roll += fmax(fmin(Kdfilt, (rpy[0] - roll)),-Kdfilt);
    pitch += fmax(fmin(Kdfilt, (rpy[1] - pitch)),-Kdfilt);
    yaw += fmax(fmin(Kdfilt,(rpy[2] - yaw)),-Kdfilt);

    roll = computeFilter(&filter_roll, roll);
    pitch = computeFilter(&filter_pitch, pitch);
    yaw = computeFilter(&filter_yaw, yaw);

    setReg(ROLL_VAL, roll);
    setReg(PITCH_VAL, pitch);
    setReg(YAW_VAL, yaw);
}


void initSensorsTasks(){
    
    initMpu(&myIMU);

    initFilter(&filter_gx, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_gy, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_gz, sz_1_10 , k_1_10, v_1_10);

    initFilter(&filter_gx2, sz_3_10 , k_3_10, v_3_10);
    initFilter(&filter_gy2, sz_3_10 , k_3_10, v_3_10);
    initFilter(&filter_gz2, sz_3_10 , k_3_10, v_3_10);

    initDNotchFilter(&dnotch_gx, 64, 50, 1000, 1, 10);
    initDNotchFilter(&dnotch_gy, 64, 50, 1000, 1, 10);
    initDNotchFilter(&dnotch_gz, 64, 50, 1000, 1, 10);

    initDNotchFilter(&dnotch_gx2, 64, 50, 1000, 1, 5);
    initDNotchFilter(&dnotch_gy2, 64, 50, 1000, 1, 5);
    initDNotchFilter(&dnotch_gz2, 64, 50, 1000, 1, 5);



    /*initFilter(&filter_ax, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_ay, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_az, sz_1_10 , k_1_10, v_1_10);*/

    initFilter(&filter_ax, sz_5_100 , k_5_100, v_5_100);
    initFilter(&filter_ay, sz_5_100 , k_5_100, v_5_100);
    initFilter(&filter_az, sz_5_100 , k_5_100, v_5_100);


    initDNotchFilter(&dnotch_ax, 64, 30, 1000, 1, 10);
    initDNotchFilter(&dnotch_ay, 64, 30, 1000, 1, 10);
    initDNotchFilter(&dnotch_az, 64, 30, 1000, 1, 10);
    
    initFilter(&filter_roll, sz_1_10, k_1_10, v_1_10);
    initFilter(&filter_pitch, sz_1_10, k_1_10, v_1_10);
    initFilter(&filter_yaw, sz_1_10, k_1_10, v_1_10);


    calibrateGyro(&myIMU);
    calibrateAccel(&myIMU);
    //calibrateMag(&myIMU);
    
    //accelHandle = osThreadNew(accelTask, NULL, &accelAttributes);
    addTask(&gyroTask, 1000, 1);
    addTask(&accelTask, 1000, 1);
    /*magHandle = osThreadNew(magTask, NULL, &magAttributes);
    rpyHandle = osThreadNew(rpyTask, NULL, &rpyAttributes);*/
    /*initTimer(&timer_accel, &accelInterrupt, 1000);
    initTimer(&timer_gyro, &gyroInterrupt, 1000);
    initTimer(&timer_mag, &magInterrupt, 10);
    initTimer(&timer_rpy, &rpyInterrupt, 500);*/
}