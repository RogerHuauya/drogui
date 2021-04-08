#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H


enum PIDVAR {PID_ROLL, PID_PITCH, PID_YAW, PID_X, PID_Y, PID_Z};


/**********************************
 * Calibration
 **********************************/

#define	CAL_SYS
#define	CAL_GYR
#define	CAL_ACC
#define	CAL_MAG

#define	CAL_GYR_TRG
#define	CAL_ACC_TRG
#define	CAL_MAG_TRG


#define	ROLL_OFFSET
#define	PITCH_OFFSET
#define	YAW_OFFSET

#define	GYR_X_OFF
#define	GYR_Y_OFF
#define	GYR_Z_OFF

#define	ACC_X_OFF
#define	ACC_Y_OFF
#define	ACC_Z_OFF
#define	ACC_SCALE

#define	MAG_X_OFF
#define	MAG_Y_OFF
#define	MAG_Z_OFF
#define	MAG_X_SCALE
#define	MAG_Y_SCALE
#define	MAG_Z_SCALE


/**********************************
 * Control Constants
 **********************************/

#define	ROLL_KP
#define	ROLL_KI
#define	ROLL_KD

#define	PITCH_KP
#define	PITCH_KI
#define	PITCH_KD

#define	YAW_KP
#define	YAW_KI
#define	YAW_KD

#define	X_KP
#define	X_KI
#define	X_KD

#define	Y_KP
#define	Y_KI
#define	Y_KD

#define	Z_KP
#define	Z_KI
#define	Z_KD

#define	PID_INDEX
#define	PID_VAR
#define	N_FILTER


/**********************************
 * Control Law
 **********************************/


#define	ROLL_U
#define	PITCH_U
#define	YAW_U
#define	X_U
#define	Y_U
#define	Z_U

#define	MOTOR_1
#define	MOTOR_2
#define	MOTOR_3
#define	MOTOR_4

#define	Z_MG

/**********************************
 * Control Setpoint
 **********************************/

#define	ROLL_REF
#define	PITCH_REF
#define	YAW_REF
#define	X_REF
#define	Y_REF
#define	Z_REF

#define	ROLL_REF_SIZE
#define	PITCH_REF_SIZE
#define	YAW_REF_SIZE
#define	X_REF_SIZE
#define	Y_REF_SIZE
#define	Z_REF_SIZE

#define	GYRO_X_REF
#define	GYRO_Y_REF
#define	GYRO_Z_REF

/**********************************
 * State
 **********************************/

#define	ROLL_VAL
#define	PITCH_VAL
#define	YAW_VAL
#define	X_VAL
#define	Y_VAL
#define	Z_VAL


/**********************************
 * Sensors
 **********************************/

// RPY
#define	RAW_ROLL
#define	RAW_PITCH
#define	RAW_YAW


// BMP
#define	RAW_TEMP
#define	TEMP_ABS
#define	RAW_PRESS
#define	PRESS_ABS

// GPS
#define	GPS_X
#define	GPS_Y
#define	GPS_CNT
#define	GPS_STATE
#define	GPS_AVAILABLE
#define	START

// Accelerometer
#define	ACC_X
#define	ACC_Y
#define	ACC_Z

// Gyroscope
#define	GYRO_X
#define	GYRO_Y
#define	GYRO_Z

#define	DER_GYRO_X
#define	DER_GYRO_Y



#endif 		
