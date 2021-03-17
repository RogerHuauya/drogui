#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H


enum PIDVAR {PID_ROLL, PID_PITCH, PID_YAW, PID_X, PID_Y, PID_Z};


#define CAL_SYS		0x02
#define CAL_GYR		0x04
#define CAL_ACC		0x06
#define CAL_MAG		0x08

#define ROLL_KP		0x0A
#define ROLL_KI		0x0C
#define ROLL_KD		0x0E

#define PITCH_KP		0x10
#define PITCH_KI		0x12
#define PITCH_KD		0x14

#define YAW_KP		0x16
#define YAW_KI		0x18
#define YAW_KD		0x1A

#define X_KP		0x1C
#define X_KI		0x1E
#define X_KD		0x20

#define Y_KP		0x22
#define Y_KI		0x24
#define Y_KD		0x26

#define Z_KP		0x28
#define Z_KI		0x2A
#define Z_KD		0x2C

#define ROLL_VAL		0x2E
#define PITCH_VAL		0x30
#define YAW_VAL		0x32
#define X_VAL		0x34
#define Y_VAL		0x36
#define Z_VAL		0x38

#define ROLL_U		0x3A
#define PITCH_U		0x3C
#define YAW_U		0x3E
#define X_U		0x40
#define Y_U		0x42
#define Z_U		0x44

#define Z_MG		0x46

#define ROLL_REF		0x48
#define PITCH_REF		0x4A
#define YAW_REF		0x4C
#define X_REF		0x4E
#define Y_REF		0x50
#define Z_REF		0x52

#define ROLL_REF_SIZE		0x54
#define PITCH_REF_SIZE		0x56
#define YAW_REF_SIZE		0x58
#define X_REF_SIZE		0x5A
#define Y_REF_SIZE		0x5C
#define Z_REF_SIZE		0x5E

#define TS_CONTROL		0x60
#define PID_INDEX		0x62
#define PID_VAR		0x64
#define RAW_TEMP		0x66
#define TEMP_ABS		0x68
#define RAW_PRESS		0x6A
#define PRESS_ABS		0x6C

#define GPS_X		0x6E
#define GPS_Y		0x70
#define GPS_AVAILABLE		0x72

#define ACC_X		0x74
#define ACC_Y		0x76
#define ACC_Z		0x78

#define START		0x7A

#define GYRO_X		0x7C
#define GYRO_Y		0x7E
#define GYRO_Z		0x80
#define GYRO_X_REF		0x82
#define GYRO_Y_REF		0x84
#define GYRO_Z_REF		0x86

#define DER_GYRO_X 0x88
#define DER_GYRO_Y 0x8A

#define MOTOR_1 0X8C
#define MOTOR_2 0X8E
#define MOTOR_3 0X90
#define MOTOR_4 0X92

#define CAL_GYR_TRG		0x94
#define CAL_ACC_TRG		0x96
#define CAL_MAG_TRG		0x98

#define N_FILTER 0x9A

#define ROLL_OFFSET 0x9C
#define PITCH_OFFSET 0x9E
#define YAW_OFFSET 0xA0

#define GYR_X_OFF 0xA2
#define GYR_Y_OFF 0xA4
#define GYR_Z_OFF 0xA6

#define ACC_X_OFF 0xA8
#define ACC_Y_OFF 0xAA
#define ACC_Z_OFF 0xAC
#define ACC_SCALE 0xAE

#define MAG_X_OFF 0xB0
#define MAG_Y_OFF 0xB2
#define MAG_Z_OFF 0xB4
#define MAG_X_SCALE 0xB6
#define MAG_Y_SCALE 0xB8
#define MAG_Z_SCALE 0xBA

#endif 		
