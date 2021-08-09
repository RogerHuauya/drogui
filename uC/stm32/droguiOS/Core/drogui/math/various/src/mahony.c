#include "mahony.h"
#include <stdlib.h>
#include <math.h>
#include "utils.h"
/*

//====================================================================================================
// Definitions
#define sampleFreq	500.0f			// sample frequency in Hz
#define twoKpDef	(2.0f * 2.0f)	// 2 * proportional gain
#define twoKiDef	(2.0f * 0.1f)	// 2 * integral gain

//===================================================================================================
// Variable definitions

volatile float twoKp = twoKpDef;											// 2 * proportional gain (Kp)
volatile float twoKi = twoKiDef;											// 2 * integral gain (Ki)
volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;					// quaternion of sensor frame relative to auxiliary frame
volatile float integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;	// integral error terms scaled by Ki

//====================================================================================================
// Function declarations
*/
float invSqrt(float x);

//====================================================================================================
// Functions

//----------------------------------------------------------------------------------------------------
// AHRS algorithm update


void initMahony(mahony *m, float kp, float ki, float sampleFreq){
	m->integralFBx = m->integralFBy = m->integralFBz = 0;
	m->q0 = 1;
	m->q1 = m->q2 = m->q3 = 0;
	m->twoKi = 2 * ki;
	m->twoKp = 2 * kp;
	m->sampleFreq = sampleFreq;
}



void mahonyUpdate(mahony *m, float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
	float recipNorm;
	float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
	float hx, hy, bx, bz;
	float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if( ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f) ) || isnan(mx) || isnan(my) || isnan(mz)) {
		mahonyUpdateIMU(m, gx, gy, gz, ax, ay, az);
		return;
	}

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Normalise magnetometer measurement
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		q0q0 = (m->q0) * (m->q0);
		q0q1 = (m->q0) * (m->q1);
		q0q2 = (m->q0) * (m->q2);
		q0q3 = (m->q0) * (m->q3);
		q1q1 = (m->q1) * (m->q1);
		q1q2 = (m->q1) * (m->q2);
		q1q3 = (m->q1) * (m->q3);
		q2q2 = (m->q2) * (m->q2);
		q2q3 = (m->q2) * (m->q3);
		q3q3 = (m->q3) * (m->q3);

		// Reference direction of Earth's magnetic field
		hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
		hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
		bx = sqrt(hx * hx + hy * hy);
		bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

		// Estimated direction of gravity and magnetic field
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;
		halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
		halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
		halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

		// Error is sum of cross product between estimated direction and measured direction of field vectors
		halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
		halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
		halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);

		// Compute and apply integral feedback if enabled
		if((m->twoKi) > 0.0f) {
			(m->integralFBx) += (m->twoKi) * halfex * (1.0f / (m->sampleFreq));	// integral error scaled by Ki
			(m->integralFBy) += (m->twoKi) * halfey * (1.0f / (m->sampleFreq));
			(m->integralFBz) += (m->twoKi) * halfez * (1.0f / (m->sampleFreq));
			gx += (m->integralFBx);	// apply integral feedback
			gy += (m->integralFBy);
			gz += (m->integralFBz);
		}
		else {
			(m->integralFBx) = 0.0f;	// prevent integral windup
			(m->integralFBy) = 0.0f;
			(m->integralFBz) = 0.0f;
		}

		// Apply proportional feedback
		gx += (m->twoKp) * halfex;
		gy += (m->twoKp) * halfey;
		gz += (m->twoKp) * halfez;
	}

	// Integrate rate of change of quaternion
	gx *= (0.5f * (1.0f / (m->sampleFreq)));		// pre-multiply common factors
	gy *= (0.5f * (1.0f / (m->sampleFreq)));
	gz *= (0.5f * (1.0f / (m->sampleFreq)));
	qa = (m->q0);
	qb = (m->q1);
	qc = (m->q2);
	m -> q0 += (-qb * gx - qc * gy - m->q3 * gz);
	m -> q1 += (qa * gx + qc * gz - m->q3 * gy);
	m -> q2 += (qa * gy - qb * gz + m->q3 * gx);
	m -> q3 += (qa * gz + qb * gy - qc * gx);

	// Normalise quaternion
	recipNorm = invSqrt((m->q0) * (m->q0) + (m->q1) * (m->q1) + (m->q2) * (m->q2) + (m->q3) * (m->q3));
	m->q0 *= recipNorm;
	m->q1 *= recipNorm;
	m->q2 *= recipNorm;
	m->q3 *= recipNorm;
}

//---------------------------------------------------------------------------------------------------
// IMU algorithm update

void mahonyUpdateIMU(mahony *m, float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Estimated direction of gravity and vector perpendicular to magnetic flux
		halfvx = (m->q1) * (m->q3) - (m->q0) * (m->q2);
		halfvy = (m->q0) * (m->q1) + (m->q2) * (m->q3);
		halfvz = (m->q0) * (m->q0) - 0.5f + (m->q3) * (m->q3);

		// Error is sum of cross product between estimated and measured direction of gravity
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
		if((m->twoKi) > 0.0f) {
			(m->integralFBx) += (m->twoKi) * halfex * (1.0f / (m->sampleFreq));	// integral error scaled by Ki
			(m->integralFBy) += (m->twoKi) * halfey * (1.0f / (m->sampleFreq));
			(m->integralFBz) += (m->twoKi) * halfez * (1.0f / (m->sampleFreq));
			gx += (m->integralFBx);	// apply integral feedback
			gy += (m->integralFBy);
			gz += (m->integralFBz);
		}
		else {
			(m->integralFBx) = 0.0f;	// prevent integral windup
			(m->integralFBy) = 0.0f;
			(m->integralFBz) = 0.0f;
		}

		// Apply proportional feedback
		gx += (m->twoKp) * halfex;
		gy += (m->twoKp) * halfey;
		gz += (m->twoKp) * halfez;
	}

	// Integrate rate of change of quaternion
	gx *= (0.5f * (1.0f / (m->sampleFreq)));		// pre-multiply common factors
	gy *= (0.5f * (1.0f / (m->sampleFreq)));
	gz *= (0.5f * (1.0f / (m->sampleFreq)));
	qa = (m->q0);
	qb = (m->q1);
	qc = (m->q2);
	(m->q0) += (-qb * gx - qc * gy - (m->q3) * gz);
	(m->q1) += (qa * gx + qc * gz - (m->q3) * gy);
	(m->q2) += (qa * gy - qb * gz + (m->q3) * gx);
	(m->q3) += (qa * gz + qb * gy - qc * gx);

	// Normalise quaternion
	recipNorm = invSqrt((m->q0) * (m->q0) + (m->q1) * (m->q1) + (m->q2) * (m->q2) + (m->q3) * (m->q3));
	(m->q0) *= recipNorm;
	(m->q1) *= recipNorm;
	(m->q2) *= recipNorm;
	(m->q3) *= recipNorm;
}

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

void getMahonyQuaternion(mahony *m, float* quat){
	quat[0] = (m->q0);
	quat[1] = (m->q1);
	quat[2] = (m->q2);
	quat[3] = (m->q3);
}


void getMahonyEuler(mahony *m, float* euler){

	// roll (x-axis rotation)
	float sinr_cosp = 2 * ((m->q0) * (m->q1) + (m->q2) * (m->q3));
	float cosr_cosp = 1 - 2 * ((m->q1) * (m->q1) + (m->q2) * (m->q2));
	euler[0] = atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = 2 * ((m->q0) * (m->q2) - (m->q3) * (m->q1));
	if (fabs(sinp) >= 1)
		euler[1] = copysign(pi / 2, sinp); // use 90 degrees if out of range
	else
		euler[1] = asin(sinp);

	// yaw (z-axis rotation)
	float siny_cosp = 2 * ((m->q0) * (m->q3) + (m->q1) * (m->q2));
	float cosy_cosp = 1 - 2 * ((m->q2) * (m->q2) + (m->q3) * (m->q3));
	euler[2] = atan2(siny_cosp, cosy_cosp);
}
