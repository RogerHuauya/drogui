#include "macros.h"

#ifdef ICM20948

#ifndef ICM_20948_H
#define ICM_20948_H

#include <stdbool.h>
#include "filter.h"

// https://3cfeqx1hf82y3xcoull08ihx-wpengine.netdna-ssl.com/wp-content/uploads/2016/06/DS-000189-ICM-20948-v1.3.pdf

#define ICM20948_ADDRESS 0x69
#define ICM_MAG_ADDRESS 0x0C
#define CHANGE_BANK 0x7F
#define PWR_MGT 0x06
#define INT_PIN_CFG 0x0F
#define USER_CTRL 0x03

enum ICM_DATA_REGS{
    ICM_ACCEL_XOUT_H = 0x2D,
    ICM_ACCEL_XOUT_L = 0x2E,
    ICM_ACCEL_YOUT_H = 0x2F,
    ICM_ACCEL_YOUT_L = 0x30,
    ICM_ACCEL_ZOUT_H = 0x31,
    ICM_ACCEL_ZOUT_L = 0x32,

    ICM_GYRO_XOUT_H = 0x33,
    ICM_GYRO_XOUT_L = 0x34,
    ICM_GYRO_YOUT_H = 0x35,
    ICM_GYRO_YOUT_L = 0x36,
    ICM_GYRO_ZOUT_H = 0x37,
    ICM_GYRO_ZOUT_L = 0x38,
};


// Bank 2
enum ICM_CONFIGS{
    ICM_GYRO_SMPLRT_DIV = 0x00,
    ICM_GYRO_CONFIG1 = 0x01,
    ICM_GYRO_CONFIG2 = 0x02,
    ICM_ACCEL_SMPLRT_DIV_MSB = 0x10,
    ICM_ACCEL_SMPLRT_DIV_LSB = 0x11,
    ICM_ACCEL_CONFIG1 = 0x14,
    ICM_ACCEL_CONFIG2 = 0x15
};

enum ICM_GYRO_SCALE{
    ICM_GYRO_FULL_SCALE_250_DPS  = 0x00,
    ICM_GYRO_FULL_SCALE_500_DPS  = 0x02,    
    ICM_GYRO_FULL_SCALE_1000_DPS =  0x04,
    ICM_GYRO_FULL_SCALE_2000_DPS =  0x06
};

enum ICM_ACCEL_SCALE{
    ICM_ACC_FULL_SCALE_2_G  = 0x00, 
    ICM_ACC_FULL_SCALE_4_G  = 0x02,
    ICM_ACC_FULL_SCALE_8_G  = 0x04,
    ICM_ACC_FULL_SCALE_16_G = 0x06
};

typedef struct _filtGyro{
    
    filter first, second, fifth;
    dNotchFilter third, fourth;

} filtGyro;

typedef struct _filtAcc{
    
    filter first;
    dNotchFilter second;

} filtAcc;

typedef struct _icm20948{  
    
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

} icm20948;

typedef icm20948 imu;

//TODO: fix comments



void initFiltGyro(filtGyro *fg);
void initFiltAcc(filtAcc *fa);


float computeFiltGyro(filtGyro *fg, float val);
float computeFiltAcc(filtAcc *fa, float val);


void cleanFiltGyro(filtGyro *fg);
void cleanFiltAcc(filtAcc *fa);



void initImu(icm20948* m);

void readRawAcc(icm20948* m);

void readFiltAcc(icm20948* m);

void readAcc(icm20948* m);

void readRawGyro(icm20948* m);

void readFiltGyro(icm20948* m);

void readGyro(icm20948* m);

void readMag(icm20948* m);

void readRawMag(icm20948* m);

void calibrateGyro(icm20948* m);

void calibrateAccel(icm20948* m);

void calibrateMag(icm20948* m);

int updateCalibOffset(icm20948* m);

#endif

#endif