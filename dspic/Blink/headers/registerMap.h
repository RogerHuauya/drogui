#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H

#define M1_VAL   0x01
#define M2_VAL   0x05
#define M3_VAL   0x09
#define M4_VAL   0x0D

#define ROLL_KP   0x11
#define ROLL_KI   0x15  
#define ROLL_KD   0x19

#define PITCH_KP  0x1D
#define PITCH_KI  0X21
#define PITCH_KD  0x25

#define YAW_KP    0x29
#define YAW_KI    0x2D
#define YAW_KD    0x31

#define ROLL_DEG  0x35
#define PITCH_DEG 0x39
#define YAW_DEG   0x3D

#define ROLL_SEN    0x41
#define PITCH_SEN   0x45
#define YAW_SEN     0x49

#define H_VAL 0x4D
#define H_STEP_SIZE 0x51
#define TS_CONTROL 0X55

#define PID_INDEX 0x59

#define GYRO_X 0x5D
#define GYRO_Y 0x61
#define GYRO_Z 0x65
#endif