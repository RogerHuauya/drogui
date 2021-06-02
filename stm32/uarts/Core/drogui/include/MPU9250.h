#include "macros.h"
#ifdef MPU9250

#ifndef MPU9250_H
#define MPU9250_H

#include <stdbool.h>
#include "filter.h"
#define MPU9250_ADDRESS 0x68
#define MPU_MAG_ADDRESS 0x0C

enum MPU_CONFIGS{
    MPU_MASTER_CONFIG = 26,
    MPU_GYRO_CONFIG   = 27,
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

typedef struct _filtGyro{
    
    filter first, second;
    dNotchFilter third, fourth;

} filtGyro;

typedef struct _filtAcc{
    
    filter first;
    dNotchFilter second;

} filtAcc;

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

    filtGyro fGyroX, fGyroY, fGyroZ;
    filtAcc fAccX, fAccY, fAccZ;

} mpu9250;


typedef mpu9250 imu;

//TODO: fix comments



void initFiltGyro(filtGyro *fg);
void initFiltAcc(filtAcc *fa);


float computeFiltGyro(filtGyro *fg, float val);
float computeFiltAcc(filtAcc *fa, float val);


void cleanFiltGyro(filtGyro *fg);
void cleanFiltAcc(filtAcc *fa);


/**
 * @brief Initialize MPU9250
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void initImu(mpu9250* m);


/**
 * @brief Read raw data from Accelerometer 
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readRawAcc(mpu9250* m);

/**
 * @brief Read raw data from Accelerometer 
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readFiltAcc(mpu9250* m);
/**
 * @brief Get data transformed from Accelerometer due to offset and scale     
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readAcc(mpu9250* m);



/**
 * @brief Read raw data from Gyroscope 
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readRawGyro(mpu9250* m);
/**
 * @brief Read raw data from Gyroscope 
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readFiltGyro(mpu9250* m);
/**
 * @brief Get data transformed from Gyroscope due to offset and scale
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readGyro(mpu9250* m);


/**
 * @brief Get data transformed from Magnetometer due to offset and scale
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readMag(mpu9250* m);
/**
 * @brief Read raw data from Magnetometer
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void readRawMag(mpu9250* m);

/**
 * @brief Calibrate Gyroscope taking data, for this the system must be quiet  
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void calibrateGyro(mpu9250* m);
/**
 * @brief Calibrate Accelerometer, for this the system must be quiet and in 6 different positions
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void calibrateAccel(mpu9250* m);
/**
 * @brief Calibrate Magnetometer, for this the system must move in 100 different positions
 * @param m MPU9250 (pointer to struct mpu9250)
*/
void calibrateMag(mpu9250* m);

int updateCalibOffset(mpu9250* m);

#endif

#endif
