#include "sensorsTasks.h"
#include "_freertos.h"
#include "mahony.h"


mpu9250 myIMU;

filter filter_gx, filter_gy, filter_gz;
filter filter_gx2, filter_gy2, filter_gz2;
dNotchFilter dnotch_gx, dnotch_gy, dnotch_gz; 
dNotchFilter dnotch_gx2, dnotch_gy2, dnotch_gz2;


filter filter_ax, filter_ay, filter_az;
dNotchFilter dnotch_ax, dnotch_ay, dnotch_az; 

filter filter_roll, filter_pitch, filter_yaw;
float roll, pitch, yaw, ax, ay, az, gx, gy, gz, mx, my, mz, x, y, z;


float state[4] = {};
arm_iir_lattice_instance_f32 f;

void accelTask(){
    for(;;){
        readAcc(&myIMU);
        
        ax = computeFilter(&filter_ax, myIMU.ax);
        ay = computeFilter(&filter_ay, myIMU.ay);
        az = computeFilter(&filter_az, myIMU.az);
        
        
        ax = computeDNotch(&dnotch_ax, ax);
        ay = computeDNotch(&dnotch_ay, ay);
        az = computeDNotch(&dnotch_az, az);
        
        setReg(ACC_X,(float)(ax));
        setReg(ACC_Y,(float)(ay));
        setReg(ACC_Z,(float)(az));
        osDelay(10);
    }
}

void gyroTask(){
    for(;;){
        readGyro(&myIMU);
        float aux = myIMU.gx;
        arm_iir_lattice_f32(&f, &aux, &gx, 1);
        gy = computeFilter(&filter_gx, myIMU.gx);
        /*gy = computeFilter(&filter_gy, myIMU.gy);
        gz = computeFilter(&filter_gz, myIMU.gz);

        
        gx = computeFilter(&filter_gx2, gx);
        gy = computeFilter(&filter_gy2, gy);
        gz = computeFilter(&filter_gz2, gz);

        gx = computeDNotch(&dnotch_gx, gx);
        gy = computeDNotch(&dnotch_gy, gy);
        gz = computeDNotch(&dnotch_gz, gz);

        gx = computeDNotch(&dnotch_gx2, gx);
        gy = computeDNotch(&dnotch_gy2, gy);
        gz = computeDNotch(&dnotch_gz2, gz);*/
        
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

float Kdfilt = 0.0005;
void rpyTask(){
    
    float rpy[3];
    while(1){
    
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
        osDelay(2);
    }
}






osThreadId_t accelHandle;
const osThreadAttr_t accelAttributes = {    .name = "accelTask", \
                                            .stack_size = 128 * 4, \
                                            .priority = (osPriority_t) osPriorityHigh7};


osThreadId_t gyroHandle;
const osThreadAttr_t gyroAttributes = {    .name = "gyroTask",\
                                            .stack_size = 256 * 4,\
                                            .priority = (osPriority_t) osPriorityHigh7};

osThreadId_t magHandle;
const osThreadAttr_t magAttributes = {    .name = "magTask",\
                                            .stack_size = 128 * 4,\
                                            .priority = (osPriority_t) osPriorityHigh7};

osThreadId_t rpyHandle;
const osThreadAttr_t rpyAttributes = {    .name = "rpyTask",\
                                            .stack_size = 128 * 4,\
                                            .priority = (osPriority_t) osPriorityHigh6};

void initSensorsTasks(){
    
    initMpu(&myIMU);
    
    arm_iir_lattice_init_f32(&f, sz_1_10 , k_1_10, v_1_10, state, 1);

    initFilter(&filter_gx, sz_1_10 , k_1_10, v_1_10);
 /*   initFilter(&filter_gy, sz_1_10 , k_1_10, v_1_10);
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



    initFilter(&filter_ax, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_ay, sz_1_10 , k_1_10, v_1_10);
    initFilter(&filter_az, sz_1_10 , k_1_10, v_1_10);

    initDNotchFilter(&dnotch_ax, 64, 40, 1000, 1, 10);
    initDNotchFilter(&dnotch_ay, 64, 40, 1000, 1, 10);
    initDNotchFilter(&dnotch_az, 64, 40, 1000, 1, 10);
    
    initFilter(&filter_roll, sz_1_10, k_1_10, v_1_10);
    initFilter(&filter_pitch, sz_1_10, k_1_10, v_1_10);
    initFilter(&filter_yaw, sz_1_10, k_1_10, v_1_10);*/


    calibrateGyro(&myIMU);
    //calibrateAccel(&myIMU);
    //calibrateMag(&myIMU);
    
    //accelHandle = osThreadNew(accelTask, NULL, &accelAttributes);
    gyroHandle = osThreadNew(gyroTask, NULL, &gyroAttributes);
    /*magHandle = osThreadNew(magTask, NULL, &magAttributes);
    rpyHandle = osThreadNew(rpyTask, NULL, &rpyAttributes);*/
    /*initTimer(&timer_accel, &accelInterrupt, 1000);
    initTimer(&timer_gyro, &gyroInterrupt, 1000);
    initTimer(&timer_mag, &magInterrupt, 10);
    initTimer(&timer_rpy, &rpyInterrupt, 500);*/
}