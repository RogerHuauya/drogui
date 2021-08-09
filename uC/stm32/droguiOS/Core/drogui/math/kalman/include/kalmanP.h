#include "macros.h"
#ifdef KALMANP

#ifndef KALMANP_H
#define KALMANP_H

#include "utils.h"
#include "matlib.h"

typedef struct _kalmanP{
	float Ts, N;
	mat p, v, Rq, Rc, u, s;
	mat Fm, Gm, Hm, bias_p, bias_v, bias_u,Pm, Q12, ye, KalmanGain, p_gps, v_gps, delta;
} kalmanP;

typedef kalmanP kalman;

/**
 * @brief Initialize Gm, Hm, Fm, Pm,p _gps, bias, p, v, u, s, ye, Rq matrices
*/
void initMatGlobal(kalmanP *kp);
/**
 * @brief Compute Kynematics with Rq and v matrices to get estimated position matrix

*/
void kynematics(kalmanP *kp);
/**
 * @brief Get Fm matrix (3x3) from rotation matrix
 */
void getMatFm(kalmanP *kp);
/**
 * @brief Get Gm matrix (3x3) from rotation matrix
 */
void getMatGm(kalmanP *kp);
/**
 * @brief Update Pm matrix with Fm and Gm matrices
 */
void UpdatePm(kalmanP *kp);
/**
 * @brief Get Kalman Gain from multiplication of Pm,Hm and Rc matrices
 */
void getKalmanGain(kalmanP *kp);
/**
 * @brief Update Covariance matrix of Gps
 */
void UpdatePmCovGPS(kalmanP *kp);
/**
 * @brief Get bias of position and velocity from current y and estimate y
 */
void getBias(kalmanP *kp);
/**
 * @brief Estimate new position with the rotation matrix and accelerometer data
 * @param ax Accelerometer axis x(float)
 * @param ay Accelerometer axis y (float)
 * @param az Accelerometer axis z(float)
 * @param roll (float)
 * @param pitch (float)
 * @param yaw (float)
 */
void kalmanUpdateIMU(kalmanP *kp, float ax, float ay, float az, float roll, float pitch,float yaw);
/**
 * @brief Estimate new position with the rotation matrix and accelerometer data
 * @param x_gps (float)
 * @param y_gps (float)
 * @param z_gps (float)
 */
void kalmanUpdateGPS(kalmanP *kp, float x_gps, float y_gps, float z_gps);
/**
 * @brief Set sampling time
 * @param ts (float)
 */
void setKalmanTsImu(kalmanP *kp, float ts);
/**
 * @brief Get position from p matrix
 * @param x X position (float*)
 * @param y Y position (float*)
 * @param z Z position (float*)
 */
void getPosition(kalmanP *kp, float *x, float *y, float *z);
/**
 * @brief Set number of samples
 */
void getVelocity(kalmanP *kp, float *xp, float *yp);
/**
 * @brief Set number of samples
 */
void setKalmanTsGps(kalmanP *kp, float ts);
/**
 * @brief Set all matrices with zeros
 */
void clearKalman(kalmanP *kp);


#endif
#endif
