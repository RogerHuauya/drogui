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
/**
 * @brief Structure gyroscope filter
 * @param first First filter type normal (structure filter)
 * @param second Second filter type normal filter (structure filter)
 * @param thrid Thrid filter type Dynamic Notch (structure dNotchFilter)
 * @param fourth Fourth filter type Dynamic Notch (structure dNotchFilter)
*/
typedef struct _filtGyro{

    filter first, second;
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
 * @brief Structure MPU9250
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
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void cleanFiltGyro(filtGyro *fg);

/**
 * @brief Clean accelerometer filter
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void cleanFiltAcc(filtAcc *fa);

/**
 * @brief Initialize MPU9250
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void initImu(mpu9250* m);


/**
 * @brief Read raw data from Accelerometer
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void readRawAcc(mpu9250* m);

/**
 * @brief Read filtered data from Accelerometer
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void readFiltAcc(mpu9250* m);

/**
 * @brief Get data transformed from Accelerometer due to offset and scale
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void readAcc(mpu9250* m);

/**
 * @brief Read raw data from Gyroscope
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void readRawGyro(mpu9250* m);

/**
 * @brief Read filtered data from Gyroscope
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void readFiltGyro(mpu9250* m);

/**
 * @brief Get data transformed from Gyroscope due to offset and scale
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void readGyro(mpu9250* m);

/**
 * @brief Get data transformed from Magnetometer due to offset and scale
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void readMag(mpu9250* m);

/**
 * @brief Read raw data from Magnetometer
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void readRawMag(mpu9250* m);

/**
 * @brief Calibrate Gyroscope taking data, for this the system must be quiet
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void calibrateGyro(mpu9250* m);

/**
 * @brief Calibrate Accelerometer, for this the system must be quiet and in 6 different positions
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void calibrateAccel(mpu9250* m);

/**
 * @brief Calibrate Magnetometer, for this the system must move in 100 different positions
 * @param m MPU9250 (pointer of struct mpu9250)
*/
void calibrateMag(mpu9250* m);

/**
 * @brief Update offsets and scales of all sensors
 * @param m MPU9250 (pointer of struct mpu9250)
 * @return
*/
int updateCalibOffset(mpu9250* m);

#endif

#endif
