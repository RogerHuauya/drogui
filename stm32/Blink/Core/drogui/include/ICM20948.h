#ifndef ICM_20948
#define ICM_20948

#include <stdbool.h>
#include "filter.h"
#define ICM20948_ADDRESS 0x68
#define ICM_MAG_ADDRESS 0x0C


enum ICM_DATA_REGS{
    ICM_ACCEL_XOUT_H = 0x2D,
    ICM_ACCEL_XOUT_L = 0x2E,
    ICM_ACCEL_YOUT_H = 0x2F,
    ICM_ACCEL_YOUT_L = 0x30,
    ICM_ACCEL_ZOUT_H = 0x31,
    ICM_ACCEL_ZOUT_L = 0x32,

    ICM_GYRO_XOUT_L = 0x33,
    ICM_GYRO_XOUT_H = 0x34,
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
    GYRO_FULL_SCALE_250_DPS  = 0x00,
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

typedef struct _icmFiltGyro{
    
    filter first, second;
    dNotchFilter third, fourth;

} icmFiltGyro;

typedef struct _icmFiltAcc{
    
    filter first;
    dNotchFilter second;

} icmFiltAcc;

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

    icmFiltGyro fGyroX, fGyroY, fGyroZ;
    icmFiltAcc fAccX, fAccY, fAccZ;

} icm20948;

//TODO: fix comments



void initIcmFiltGyro(icmFiltGyro *fg);
void initIcmFiltAcc(icmFiltAcc *fa);


float computeIcmFiltGyro(icmFiltGyro *fg, float val);
float computeIcmFiltAcc(icmFiltAcc *fa, float val);


void cleanIcmFiltGyro(icmFiltGyro *fg);
void cleanIcmFiltAcc(icmFiltAcc *fa);



void initIcm(icm20948* m);

void readIcmRawAcc(icm20948* m);

void readIcmFiltAcc(icm20948* m);

void readIcmAcc(icm20948* m);

void readIcmRawGyro(icm20948* m);

void readIcmFiltGyro(icm20948* m);

void readIcmGyro(icm20948* m);

void readIcmMag(icm20948* m);

void readIcmRawMag(icm20948* m);

void calibrateIcmGyro(icm20948* m);

void calibrateIcmAccel(icm20948* m);

void calibrateIcmMag(icm20948* m);

int updateIcmCalibOffset(icm20948* m);

#endif