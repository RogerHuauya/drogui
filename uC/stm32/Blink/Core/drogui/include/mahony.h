#ifndef MAHONY_H
#define MAHONY_h



typedef struct _mahony{
    float sampleFreq;
    volatile float twoKp;											// 2 * proportional gain (Kp)
    volatile float twoKi;											// 2 * integral gain (Ki)
    volatile float q0, q1, q2, q3;					// quaternion of sensor frame relative to auxiliary frame
    volatile float integralFBx,  integralFBy, integralFBz;	// integral error terms scaled by Ki
} mahony;




void initMahony(mahony *m, float kp, float ki, float sampleFreq);

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
void mahonyUpdate(mahony *m, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
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
void mahonyUpdateIMU(mahony *m, float gx, float gy, float gz, float ax, float ay, float az);
/**
 * @brief Get Quaternion values (qw-qx-qy-qz)
 * @param quat Array of Quaternion values (float*)
*/
void getMahonyQuaternion(mahony *m, float* quat);
/**
 * @brief Get Euler's angles (Roll-Pitch-Yaw)
 * @param euler Array of Euler's angles (float*)
*/
void getMahonyEuler(mahony *m, float* euler);

#endif