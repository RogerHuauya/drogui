#include "macros.h"
#ifdef KALMANPV

#ifndef KALMANPV_H
#define KALMANPV_H

#include "utils.h"
#include "matlib.h"

typedef struct _kalmanPV{
	float Ts, N;
	mat p, v, Rq, Rc, u, s;
	mat Fm, Gm, Hm, bias_p, bias_v, bias_u,Pm, Q12, ye, KalmanGain, p_gps, v_gps, delta;
} kalmanPV;

typedef kalmanPV kalman;

/**
 * @brief Initialize Gm, Hm, Fm, Pm,p _gps, bias, p, v, u, s, ye, Rq matrices
*/
void initMatGlobal(kalmanPV *kpv);
/**
 * @brief Compute Kynematics with Rq and v matrices to get estimated position matrix

*/
void kynematics(kalmanPV *kpv);
/**
 * @brief Get Fm matrix (3x3) from rotation matrix
 */
void getMatFm(kalmanPV *kpv);
/**
 * @brief Get Gm matrix (3x3) from rotation matrix
 */
void getMatGm(kalmanPV *kpv);
/**
 * @brief Update Pm matrix with Fm and Gm matrices
 */
void UpdatePm(kalmanPV *kpv);
/**
 * @brief Get Kalman Gain from multiplication of Pm,Hm and Rc matrices
 */
void getKalmanGain(kalmanPV *kpv);
/**
 * @brief Update Covariance matrix of Gps
 */
void UpdatePmCovGPS(kalmanPV *kpv);
/**
 * @brief Get bias of position and velocity from current y and estimate y
 */
void getBias(kalmanPV *kpv);
/**
 * @brief Estimate new position with the rotation matrix and accelerometer data
 * @param ax Accelerometer axis x(float)
 * @param ay Accelerometer axis y (float)
 * @param az Accelerometer axis z(float)
 * @param roll (float)
 * @param pitch (float)
 * @param yaw (float)
 */
void kalmanUpdateIMU(kalmanPV *kpv, float ax, float ay, float az, float roll, float pitch,float yaw);
/**
 * @brief Estimate new position with the rotation matrix and accelerometer data
 * @param x_gps (float)
 * @param y_gps (float)
 * @param z_gps (float)
 */
void kalmanUpdateGPS(kalmanPV *kpv, float x_gps, float y_gps, float vx_gps, float vy_gps);
/**
 * @brief Set sampling time
 * @param ts (float)
 */
void setKalmanTsImu(kalmanPV *kpv, float ts);
/**
 * @brief Get position from p matrix
 * @param x X position (float*)
 * @param y Y position (float*)
 * @param z Z position (float*)
 */
void getPosition(kalmanPV *kpv, float *x, float *y);
/**
 * @brief Set number of samples
 */
void getVelocity(kalmanPV *kpv, float *xp, float *yp);
/**
 * @brief Set number of samples
 */
void setKalmanTsGps(kalmanPV *kpv, float ts);
/**
 * @brief Set all matrices with zeros
 */
void clearKalman(kalmanPV *kpv);


#endif
#endif
