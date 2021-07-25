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

#define AK09916_WHO_I_AM1 0x00
#define AK09916_WHO_I_AM2 0x01
#define AK09916_CNTL2 0x31 
#define AK09916_ST1 0x10
#define AK09916_ST2 0x18

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

typedef enum {
  AK09916_MAG_DATARATE_SHUTDOWN = 0x0, ///< Stops measurement updates
  AK09916_MAG_DATARATE_SINGLE =
      0x1, ///< Takes a single measurement then switches to
           ///< AK09916_MAG_DATARATE_SHUTDOWN
  AK09916_MAG_DATARATE_10_HZ = 0x2,  ///< updates at 10Hz
  AK09916_MAG_DATARATE_20_HZ = 0x4,  ///< updates at 20Hz
  AK09916_MAG_DATARATE_50_HZ = 0x6,  ///< updates at 50Hz
  AK09916_MAG_DATARATE_100_HZ = 0x8, ///< updates at 100Hz
} ak09916_data_rate_t;


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

/**
 * @brief Structure gyroscope filter   
 * @param first First filter type normal (structure filter)
 * @param second Second filter type normal filter (structure filter)
 * @param thrid Thrid filter type Dynamic Notch (structure dNotchFilter)
 * @param fourth Fourth filter type Dynamic Notch (structure dNotchFilter)
*/
typedef struct _filtGyro{
    
    filter first, second, fifth;
    dNotchFilter third, fourth;

} filtGyro;

/**
 * @brief Structure accelerometer filter   
 * @param first First filter type normal (structure filter)
 * @param second Second filter type Dynamic Notch (structure dNotchFilter)
*/
typedef struct _filtAcc{
    
    filter first;
    dNotchFilter second;

} filtAcc;

/**
 * @brief Structure ICM20948  
 * @param ax Accelerometer value in x direction (float)
 * @param ay Accelerometer value in y direction (float)
 * @param az Accelerometer value in z direction (float)
 * @param gx Gyroscope value in x direction (float)
 * @param gy Gyroscope value in y direction (float)
 * @param gz Gyroscope value in z direction (float)
 * @param mx Magnetometer value in x direction (float)
 * @param my Magnetometer value in y direction (float)
 * @param mz Magnetometer value in z direction (float) 
 * @param raw_ax Raw Accelerometer value in x direction (float)
 * @param raw_ay Raw Accelerometer value in y direction (float)
 * @param raw_az Raw Accelerometer value in z direction (float)
 * @param raw_gx Raw Gyroscope value in x direction (float)
 * @param raw_gy Raw Gyroscope value in y direction (float)
 * @param raw_gz Raw Gyroscope value in z direction (float)
 * @param raw_mx Raw Magnetometer value in x direction (float)
 * @param raw_my Raw Magnetometer value in y direction (float)
 * @param raw_mz Raw Magnetometer value in z direction (float) 
 * @param filt_ax Filtered Accelerometer value in x direction (float)
 * @param filt_ay Filtered Accelerometer value in y direction (float)
 * @param filt_az Filtered Accelerometer value in z direction (float)
 * @param filt_gx Filtered Gyroscope value in x direction (float)
 * @param filt_gy Filtered Gyroscope value in y direction (float)
 * @param filt_gz Filtered Gyroscope value in z direction (float)
 * @param off_ax Offset of Accelerometer value in x direction (float)
 * @param off_ay Offset of Accelerometer value in y direction (float)
 * @param off_az Offset of Accelerometer value in z direction (float)
 * @param scl_ac Scale factor of accelerometer (float)
 * @param off_gx Offset of Gyroscope value in x direction (float)
 * @param off_gy Offset of Gyroscope value in y direction (float)
 * @param off_gz Offset of Gyroscope value in z direction (float)
 * @param off_mx Offset of Magnetometer value in x direction (float)
 * @param off_my Offset of Magnetometer value in y direction (float)
 * @param off_mz Offset of Magnetometer value in z direction (float)
 * @param scl_magx Scale factor of magnetometer in x direction (float)
 * @param scl_magy Scale factor of magnetometer in y direction (float)
 * @param scl_magz Scale factor of magnetometer in z direction (float)
 * @param accScale Scale factor which depends on the configuration of sensor (float)
 * @param gyroScale Scale factor which depends on the configuration of sensor (float)
 * @param magScale Scale factor which depends on the configuration of sensor (float)
 * @param isGyroCalibrated Describe if gyroscope is calibrated (bool)
 * @param isAccelGyroCalibrated Describe if accelerometer is calibrated (bool)
 * @param isMagCalibrated Describe if magnetometer is calibrated (bool)
 * @param fGyroX Gyroscope filter in x direction (structure filter)
 * @param fGyroY Gyroscope filter in y direction (structure filter)
 * @param fGyroZ Gyroscope filter in z direction (structure filter)
 * @param fAccX Accelerometer filter x direction (structure filter)
 * @param fAccY Accelerometer filter y direction (structure filter)
 * @param fAccZ Accelerometer filter z direction (structure filter)
*/
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

/**
 * @brief Initialize gyroscope filter   
 * @param fg (pointer of structure filtGyro)
*/
void initFiltGyro(filtGyro *fg);

/**
 * @brief Initialize accelerometer filter   
 * @param fa (pointer of structure filtAcc)
*/
void initFiltAcc(filtAcc *fa);

/**
 * @brief Compute acclerometer filter  
 * @param fg (pointer of structure filtGyro)
 * @param val Current sensor value (float)
 * @return Filtered current value
*/
float computeFiltGyro(filtGyro *fg, float val);

/**
 * @brief Compute acclerometer filter  
 * @param fa (pointer of structure filtAcc)
 * @param val Current sensor value (float)
 * @return Filtered current value
*/
float computeFiltAcc(filtAcc *fa, float val);

/**
 * @brief Clean gyroscope filter 
 * @param m ICM20948 (pointer of struct icm20948)
*/
void cleanFiltGyro(filtGyro *fg);

/**
 * @brief Clean accelerometer filter 
 * @param m ICM20948 (pointer of struct icm20948)
*/
void cleanFiltAcc(filtAcc *fa);


/**
 * @brief Initialize ICM20948
 * @param m ICM20948 (pointer of struct icm20948)
*/
void initImu(icm20948* m);

/**
 * @brief Read raw data from Accelerometer 
 * @param m ICM20948 (pointer of struct icm20948)
*/
void readRawAcc(icm20948* m);

/**
 * @brief Read filtered data from Accelerometer 
 * @param m ICM20948 (pointer of struct icm20948)
*/
void readFiltAcc(icm20948* m);

/**
 * @brief Get data transformed from Accelerometer due to offset and scale     
 * @param m ICM20948 (pointer of struct icm20948)
*/
void readAcc(icm20948* m);

/**
 * @brief Read raw data from Gyroscope 
 * @param m ICM20948 (pointer of struct icm20948)
*/
void readRawGyro(icm20948* m);

/**
 * @brief Read filtered data from Gyroscope 
 * @param m ICM20948 (pointer of struct icm20948)
*/
void readFiltGyro(icm20948* m);

/**
 * @brief Get data transformed from Gyroscope due to offset and scale
 * @param m ICM20948 (pointer of struct icm20948)
*/
void readGyro(icm20948* m);

/**
 * @brief Get data transformed from Magnetometer due to offset and scale
 * @param m ICM20948 (pointer of struct icm20948)
*/
void readMag(icm20948* m);

/**
 * @brief Read raw data from Magnetometer
 * @param m ICM20948 (pointer of struct icm20948)
*/
void readRawMag(icm20948* m);

/**
 * @brief Calibrate Gyroscope taking data, for this the system must be quiet  
 * @param m ICM20948 (pointer of struct icm20948)
*/
void calibrateGyro(icm20948* m);

/**
 * @brief Calibrate Accelerometer, for this the system must be quiet and in 6 different positions
 * @param m ICM20948 (pointer of struct icm20948)
*/
void calibrateAccel(icm20948* m);

/**
 * @brief Calibrate Magnetometer, for this the system must move in 100 different positions
 * @param m ICM20948 (pointer of struct icm20948)
*/
void calibrateMag(icm20948* m);

/**
 * @brief Update offsets and scales of all sensors 
 * @param m ICM20948 (pointer of struct icm20948)
 * @return 
*/
int updateCalibOffset(icm20948* m);

#endif

#endif
