#ifndef REGISTER_MAP_H
#define REGISTER_MAP_H

<<<<<<< HEAD
#define NUM_THREADS 2


#define RmoveM1   0x01
#define RmoveM2   0x02
#define RmoveM3   0x03
#define RmoveM4   0x04
#define RcalibM   0x05
#define RpauseM   0x06
#define Rstop     0x07
#define RpX       0x08
#define RpY       0X09
#define RpZ       0x10
#define RdiffX    0x11
#define RdiffY    0x12
#define RdiffZ    0x13
#define Ralpha    0x14
#define Rpresion  0x15
#define Rroll     0X0C
#define Rpitch    0X0D
#define Ryaw      0x0E
#define Rbateria  0x19 
=======
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
>>>>>>> roger

#endif