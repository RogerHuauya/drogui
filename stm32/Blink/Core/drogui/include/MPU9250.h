#ifndef MPU9250_H
#define MPU9250_H

#include <stdbool.h>
#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C

enum MPU_CONFIGS{
    MASTER_CONFIG = 26,
    GYRO_CONFIG = 27,
    ACCEL_CONFIG1 = 28,
    ACCEL_CONFIG2 = 29
};

enum GYRO_SCALE{
    GYRO_FULL_SCALE_250_DPS  = 0x00,
    GYRO_FULL_SCALE_500_DPS  = 0x08,    
    GYRO_FULL_SCALE_1000_DPS =  0x10,
    GYRO_FULL_SCALE_2000_DPS =  0x18
};

enum ACCEL_SCALE{
    ACC_FULL_SCALE_2_G  = 0x00, 
    ACC_FULL_SCALE_4_G  = 0x08,
    ACC_FULL_SCALE_8_G  = 0x10,
    ACC_FULL_SCALE_16_G = 0x18
};

typedef struct _mpu9250{  
    float ax, ay, az, off_ax, off_ay, off_az, scl_acc;
    float gx, gy, gz, off_gx, off_gy, off_gz;
    float mx, my, mz, off_mx, off_my, off_mz, scl_magx, scl_magy, scl_magz;
    
    float raw_ax, raw_ay, raw_az;
    float raw_gx, raw_gy, raw_gz;
    float raw_mx, raw_my, raw_mz;
    int accScale, gyroScale, magScale;
    bool isGyroCalibrated;
    bool isAccelCalibrated;
    bool isMagCalibrated;
} mpu9250;

void initMpu(mpu9250* m);
void readRawAcc(mpu9250* m);
void readAcc(mpu9250* m);
void readRawGyro(mpu9250* m);
void readGyro(mpu9250* m);
void readMag(mpu9250* m);
void readRawMag(mpu9250* m);
void calibrateGyro(mpu9250* m);
void calibrateAccel(mpu9250* m);
void calibrateMag(mpu9250* m);

#endif