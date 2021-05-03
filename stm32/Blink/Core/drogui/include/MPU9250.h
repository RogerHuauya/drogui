#ifndef MPU9250_H
#define MPU9250_H

#include <stdbool.h>
#include "filter.h"
#define MPU9250_ADDRESS 0x68
#define MPU_MAG_ADDRESS 0x0C

enum MPU_CONFIGS{
    MPU_MASTER_CONFIG = 26,
    MPU_GYRO_CONFIG = 27,
    MPU_ACCEL_CONFIG1 = 28,
    MPU_ACCEL_CONFIG2 = 29
};

enum MPU_GYRO_SCALE{
    MPU_GYRO_FULL_SCALE_250_DPS  = 0x00,
    MPU_GYRO_FULL_SCALE_500_DPS  = 0x08,    
    MPU_GYRO_FULL_SCALE_1000_DPS =  0x10,
    MPU_GYRO_FULL_SCALE_2000_DPS =  0x18
};

enum MPU_ACCEL_SCALE{
    MPU_ACC_FULL_SCALE_2_G  = 0x00, 
    MPU_ACC_FULL_SCALE_4_G  = 0x08,
    MPU_ACC_FULL_SCALE_8_G  = 0x10,
    MPU_ACC_FULL_SCALE_16_G = 0x18
};

typedef struct _mpuFiltGyro{
    
    filter first, second;
    dNotchFilter third, fourth;

} mpuFiltGyro;

typedef struct _mpuFiltAcc{
    
    filter first;
    dNotchFilter second;

} mpuFiltAcc;

typedef struct _mpu9250{  
    
    float ax, ay, az; 
    float gx, gy, gz; 
    float mx, my, mz; 
    
    float raw_ax, raw_ay, raw_az;
    float raw_gx, raw_gy, raw_gz;
    float raw_mx, raw_my, raw_mz;

    float filt_ax, filt_ay, filt_az;
    float filt_gx, filt_gy, filt_gz;
    
    float off_ax, off_ay, off_az, scl_acc;
    float off_gx, off_gy, off_gz;
    float off_mx, off_my, off_mz, scl_magx, scl_magy, scl_magz;
    
    
    
    int accScale, gyroScale, magScale;
    bool isGyroCalibrated;
    bool isAccelCalibrated;
    bool isMagCalibrated;

    mpuFiltGyro fGyroX, fGyroY, fGyroZ;
    mpuFiltAcc fAccX, fAccY, fAccZ;

} mpu9250;

//TODO: fix comments



void initMpuFiltGyro(mpuFiltGyro *fg);
void initMpuFiltAcc(mpuFiltAcc *fa);


float computeMpuFiltGyro(mpuFiltGyro *fg, float val);
float computeMpuFiltAcc(mpuFiltAcc *fa, float val);


void cleanMpuFiltGyro(mpuFiltGyro *fg);
void cleanMpuFiltAcc(mpuFiltAcc *fa);


/**
 * @brief Initialize MPU9250
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void initMpu(mpu9250* m);


/**
 * @brief Read raw data from Accelerometer 
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readMpuRawAcc(mpu9250* m);

/**
 * @brief Read raw data from Accelerometer 
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readMpuFiltAcc(mpu9250* m);
/**
 * @brief Get data transformed from Accelerometer due to offset and scale     
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readMpuAcc(mpu9250* m);



/**
 * @brief Read raw data from Gyroscope 
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readMpuRawGyro(mpu9250* m);
/**
 * @brief Read raw data from Gyroscope 
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readMpuFiltGyro(mpu9250* m);
/**
 * @brief Get data transformed from Gyroscope due to offset and scale
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readMpuGyro(mpu9250* m);


/**
 * @brief Get data transformed from Magnetometer due to offset and scale
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readMpuMag(mpu9250* m);
/**
 * @brief Read raw data from Magnetometer
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readMpuRawMag(mpu9250* m);

/**
 * @brief Calibrate Gyroscope taking data, for this the system must be quiet  
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void calibrateMpuGyro(mpu9250* m);
/**
 * @brief Calibrate Accelerometer, for this the system must be quiet and in 6 different positions
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void calibrateMpuAccel(mpu9250* m);
/**
 * @brief Calibrate Magnetometer, for this the system must move in 100 different positions
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void calibrateMpuMag(mpu9250* m);

int updateMpuCalibOffset(mpu9250* m);

#endif