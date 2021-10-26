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
#define SER_OPT &serial4
#define SER_GPS &serial5

// kalman
// ------
#define KALMANP
//#define KALMANPV

// range finder
// ------------
// #define TERARANGER
#define TF02PRO

#endif
