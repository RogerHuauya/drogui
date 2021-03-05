#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"


#define EPS 1e-9
#define pi acos(-1)
#define copysign(x, y) ( ( (((x) > 0) && ((y) < 0)) || (((x) < 0) && ((y) > 0)) ) ? (-1)*(x) : (x) )

int32_t bytestoint32(uint8_t *bytesint32);
void int32tobytes(int32_t n,uint8_t *bytesint32);
void floattobytes(float n, uint8_t* bytesfloat);
float bytestofloat(uint8_t *bytesfloat);
/**
 * @param reg registro que va a ser leido
 * @return valor en el regsitro
 */
float getReg(uint8_t reg);
void setReg(uint8_t reg, float val);
void getEuler(float q0, float q1, float q2, float q3, float* roll, float *pitch, float* yaw);
float angle_dif(float angle1, float angle2);


extern uint8_t i2cReg[300][4];
extern bool security;

// Fs = 100

//Lowpass freq = 10Hz
extern float coeffA_10Hz[];
extern float coeffB_10Hz[];

//Lowpass freq = 10Hz
extern float coeffA_8Hz[];
extern float coeffB_8Hz[];

//Lowpass freq = 2Hz
extern float coeffB_2Hz[];
extern float coeffA_2Hz[];

//Lowpass freq = 5Hz
extern float coeffB_5Hz[];
extern float coeffA_5Hz[];

// Fs = 1000 Hz

//lowpass freq = 100Hz
extern float coeffB_150Hz[];
extern float coeffA_150Hz[];

extern float coeffB_100Hz[];
extern float coeffA_100Hz[];

extern float coeffB_300Hz[];
extern float coeffA_300Hz[];

// Fs = 500Hz

extern float coeffB_50Hz[];
extern float coeffA_50Hz[];

// Fc/Fs = 1/10
extern int sz_1_10;
extern float k_1_10[];
extern float v_1_10[];


// Fc/Fs = 3/10
extern int sz_3_10;
extern float k_3_10[];
extern float v_3_10[];

// Fc/Fs = 5/100
extern int sz_5_100;
extern float k_5_100[];
extern float v_5_100[];



void rampValue(float *var, float desired, float step);

#endif
