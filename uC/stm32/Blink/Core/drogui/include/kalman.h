#ifndef KALMAN_H
#define KALMAN_H

#include "utils.h"
#include "matlib.h"
/**
 * @brief Initialize Gm, Hm, Fm, Pm,p _gps, bias, p, v, u, s, ye, Rq matrices
*/
void initMatGlobal();
/**
 * @brief Compute Kynematics with Rq and v matrices to get estimated position matrix

*/
void kynematics();
/**
 * @brief Get Fm matrix (3x3) from rotation matrix
 */
void getMatFm();
/**
 * @brief Get Gm matrix (3x3) from rotation matrix
 */
void getMatGm();
/**
 * @brief Update Pm matrix with Fm and Gm matrices
 */
void UpdatePm();
/**
 * @brief Get Kalman Gain from multiplication of Pm,Hm and Rc matrices
 */
void getKalmanGain();
/**
 * @brief Update Covariance matrix of Gps 
 */
void UpdatePmCovGPS();
/**
 * @brief Get bias of position and velocity from current y and estimate y 
 */
void getBias();
/**
 * @brief Estimate new position with the rotation matrix and accelerometer data 
 * @param ax Accelerometer axis x(float)
 * @param ay Accelerometer axis y (float)
 * @param az Accelerometer axis z(float)
 * @param roll (float)
 * @param pitch (float)
 * @param yaw (float)
 */
void kalmanUpdateIMU(float ax, float ay, float az, float roll, float pitch,float yaw);
/**
 * @brief Estimate new position with the rotation matrix and accelerometer data 
 * @param x_gps (float)
 * @param y_gps (float)
 * @param z_gps (float)
 */
void kalmanUpdateGPS(float x_gps, float y_gps, float z_gps);
/**
 * @brief Set sampling time  
 * @param ts (float)
 */
void setKalmanTsImu(float ts);
/**
 * @brief Get position from p matrix
 * @param x X position (float*)
 * @param y Y position (float*)
 * @param z Z position (float*)
 */
void getPosition(float *x, float *y, float *z);
/**
 * @brief Set number of samples  
 */
void setKalmanTsGps(float ts);
/**
 * @brief Set all matrices with zeros 
 */
void clearKalman();


#endif
