#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) > (y)) ? (y) : (x))
#define pi acos(-1)
#define copysign(x, y) ( ( (((x) > 0) && ((y) < 0)) || (((x) < 0) && ((y) > 0)) ) ? (-1)*(x) : (x) )

int32_t bytestoint32(uint8_t *bytesint32);
void int32tobytes(int32_t n,uint8_t *bytesint32);
void floattobytes(float n, uint8_t* bytesfloat);
float bytestofloat(uint8_t *bytesfloat);
float getReg(uint8_t reg);
void setReg(uint8_t reg, float val);
void getEuler(double q0, double q1, double q2, double q3, double* roll, double *pitch, double* yaw);
double angle_dif(double angle1, double angle2);

#endif
