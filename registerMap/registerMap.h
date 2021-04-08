#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H


enum PIDVAR {PID_ROLL, PID_PITCH, PID_YAW, PID_X, PID_Y, PID_Z};


/**********************************
 * Calibration
 **********************************/

#define	CAL_SYS	0x02
#define	CAL_GYR	0x04
#define	CAL_ACC	0x06
#define	CAL_MAG	0x08

#define	CAL_GYR_TRG	0x0A
#define	CAL_ACC_TRG	0x0C
#define	CAL_MAG_TRG	0x0E


#define	ROLL_OFFSET	0x10
#define	PITCH_OFFSET	0x12
#define	YAW_OFFSET	0x14

#define	GYR_X_OFF	0x16
#define	GYR_Y_OFF	0x18
#define	GYR_Z_OFF	0x1A

#define	ACC_X_OFF	0x1C
#define	ACC_Y_OFF	0x1E
#define	ACC_Z_OFF	0x20
#define	ACC_SCALE	0x22

#define	MAG_X_OFF	0x24
#define	MAG_Y_OFF	0x26
#define	MAG_Z_OFF	0x28
#define	MAG_X_SCALE	0x2A
#define	MAG_Y_SCALE	0x2C
#define	MAG_Z_SCALE	0x2E


/**********************************
 * Control Constants
 **********************************/

#define	ROLL_KP	0x30
#define	ROLL_KI	0x32
#define	ROLL_KD	0x34

#define	PITCH_KP	0x36
#define	PITCH_KI	0x38
#define	PITCH_KD	0x3A

#define	YAW_KP	0x3C
#define	YAW_KI	0x3E
#define	YAW_KD	0x40

#define	X_KP	0x42
#define	X_KI	0x44
#define	X_KD	0x46

#define	Y_KP	0x48
#define	Y_KI	0x4A
#define	Y_KD	0x4C

#define	Z_KP	0x4E
#define	Z_KI	0x50
#define	Z_KD	0x52

#define	PID_INDEX	0x54
#define	PID_VAR	0x56
#define	N_FILTER	0x58


/**********************************
 * Control Law
 **********************************/


#define	ROLL_U	0x5A
#define	PITCH_U	0x5C
#define	YAW_U	0x5E
#define	X_U	0x60
#define	Y_U	0x62
#define	Z_U	0x64

#define	MOTOR_1	0x66
#define	MOTOR_2	0x68
#define	MOTOR_3	0x6A
#define	MOTOR_4	0x6C

#define	Z_MG	0x6E

/**********************************
 * Control Setpoint
 **********************************/

#define	ROLL_REF	0x70
#define	PITCH_REF	0x72
#define	YAW_REF	0x74
#define	X_REF	0x76
#define	Y_REF	0x78
#define	Z_REF	0x7A

#define	ROLL_REF_SIZE	0x7C
#define	PITCH_REF_SIZE	0x7E
#define	YAW_REF_SIZE	0x80
#define	X_REF_SIZE	0x82
#define	Y_REF_SIZE	0x84
#define	Z_REF_SIZE	0x86

#define	GYRO_X_REF	0x88
#define	GYRO_Y_REF	0x8A
#define	GYRO_Z_REF	0x8C

/**********************************
 * State
 **********************************/

#define	ROLL_VAL	0x8E
#define	PITCH_VAL	0x90
#define	YAW_VAL	0x92
#define	X_VAL	0x94
#define	Y_VAL	0x96
#define	Z_VAL	0x98


/**********************************
 * Sensors
 **********************************/

// RPY
#define	RAW_ROLL	0x9A
#define	RAW_PITCH	0x9C
#define	RAW_YAW	0x9E


// BMP
#define	RAW_TEMP	0xA0
#define	TEMP_ABS	0xA2
#define	RAW_PRESS	0xA4
#define	PRESS_ABS	0xA6

// GPS
#define	GPS_X	0xA8
#define	GPS_Y	0xAA
#define	GPS_CNT	0xAC
#define	GPS_STATE	0xAE
#define	GPS_AVAILABLE	0xB0
#define	START	0xB2

// Accelerometer
#define	ACC_X	0xB4
#define	ACC_Y	0xB6
#define	ACC_Z	0xB8

// Gyroscope
#define	GYRO_X	0xBA
#define	GYRO_Y	0xBC
#define	GYRO_Z	0xBE

#define	DER_GYRO_X	0xC0
#define	DER_GYRO_Y	0xC2



#endif 		
