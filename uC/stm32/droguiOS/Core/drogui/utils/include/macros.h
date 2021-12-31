#ifndef MACROS_H
#define MACROS_H

// gps
// ---
#define ZED_F9P
//#define SAM_M8Q

// imu
// ---
//#define MPU9250
#define ICM20948

#define SER_DBG &serial3
#define SER_RNG &serial2
#define SER_FSK &serial4
#define SER_GPS &serial5

// kalman
// ------
//#define KALMANP
#define KALMANPV

// range finder
// ------------
// #define TERARANGER
#define TF02PRO

// radio control 
// ------------
#define FLY_RPY 1
#define FLY_WRPY 2
#define FLY_XYP 3
#define FLY_XY 4
#define FLYSKY FLY_RPY

#endif
