#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H


enum PIDVAR {PID_ROLL, PID_PITCH, PID_YAW, PID_X, PID_Y, PID_Z};


#define M1_VAL 		0x01
#define M2_VAL 		0x05
#define M3_VAL 		0x09
#define M4_VAL 		0x0D

#define ROLL_KP 		0x11
#define ROLL_KI 		0x15
#define ROLL_KD 		0x19

#define PITCH_KP 		0x1D
#define PITCH_KI 		0x21
#define PITCH_KD 		0x25

#define YAW_KP 		0x29
#define YAW_KI 		0x2D
#define YAW_KD 		0x31

#define X_KP 		0x35
#define X_KI 		0x39
#define X_KD 		0x3D

#define Y_KP 		0x41
#define Y_KI 		0x45
#define Y_KD 		0x49

#define Z_KP 		0x4D
#define Z_KI 		0x51
#define Z_KD 		0x55

#define ROLL_VAL 		0x59
#define PITCH_VAL 		0x5D
#define YAW_VAL 		0x61
#define X_VAL 		0x65
#define Y_VAL 		0x69
#define Z_VAL 		0x6D

#define ROLL_U 		0x71
#define PITCH_U 		0x75
#define YAW_U 		0x79
#define X_U 		0x7D
#define Y_U 		0x81
#define Z_U 		0x85

#define Z_MG 		0x89

#define ROLL_REF 		0x8D
#define PITCH_REF 		0x91
#define YAW_REF 		0x95
#define X_REF 		0x99
#define Y_REF 		0x9D
#define Z_REF 		0xA1

#define ROLL_REF_SIZE 		0xA5
#define PITCH_REF_SIZE 		0xA9
#define YAW_REF_SIZE 		0xAD
#define X_REF_SIZE 		0xB1
#define Y_REF_SIZE 		0xB5
#define Z_REF_SIZE 		0xB9

#define TS_CONTROL 		0xBD
#define PID_INDEX 		0xC1
#define PID_VAR          0xC5
#define RAW_TEMP 		0xC9
#define TEMP_ABS 		0xCD
#define RAW_PRESS 		0xD1
#define PRESS_ABS 		0xD5

#define GPS_X 0xD9
#define GPS_Y 0xDD
#define GPS_AVAILABLE 0XE1

#define ACC_X   0xE5
#define ACC_Y   0xE9
#define ACC_Z   0xED

#define START 0xF1

#define CALIBRATE 0xE1

#endif 		

