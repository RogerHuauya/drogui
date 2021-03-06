#ifndef MAHONY_H
#define MAHONY_h

/**
 * @brief Update Mahony
 * @param gx Gyroscope x axis
 * @param gy Gyroscope y axis
 * @param gz Gyroscope z axis
 * @param ax Accelerometer x axis
 * @param ay Accelerometer y axis
 * @param az Accelerometer z axis 
 * @param mx Magnetometer x axis
 * @param my Magnetometer y axis
 * @param mz Magnetometer z axis
*/
void mahonyUpdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
/**
 * @brief Update Mahony IMU
 * @param gx Gyroscope x axis
 * @param gy Gyroscope y axis
 * @param gz Gyroscope z axis
 * @param ax Accelerometer x axis
 * @param ay Accelerometer y axis
 * @param az Accelerometer z axis 
 * @param mx Magnetometer x axis
 * @param my Magnetometer y axis
 * @param mz Magnetometer z axis
*/
void mahonyUpdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
/**
 * @brief Get Quaternion values (qw-qx-qy-qz)
 * @param quat Array of Quaternion values (float*)
*/
void getMahonyQuaternion(float* quat);
/**
 * @brief Get Euler's angles (Roll-Pitch-Yaw)
 * @param euler Array of Euler's angles (float*)
*/
void getMahonyEuler(float* euler);

#endif