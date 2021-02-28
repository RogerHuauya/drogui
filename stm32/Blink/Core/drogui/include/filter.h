#ifndef FILTER_H
#define FILTER_H

#include "main.h"

typedef struct _arrCoeff{
    int n, head;
    float *values, *coeff;
}arrCoeff;

typedef struct _filter{
    int n;
    arrCoeff arr_u, arr_y;
}filter ;

typedef struct _dNotchFilter{
    filter f;
    float *values;
    int n;
    float fs, threshold, a, zeta;
    float num[3], den[2];
    int head;
    uint8_t exponent;
    //arduinoFFT FFT; 
}dNotchFilter;

void initFilter(filter* f, int n, float* a, float* b);
float computeFilter(filter *f, float x);

void initArrCoeff(arrCoeff* a, int n, float * coeff);
float computeArrCoeff(arrCoeff* a);
void addArrCoeff(arrCoeff* a, float x);

void initDNotchFilter(dNotchFilter* df, int n, float threshold, float fs, float a, float zeta);
float computeDNotch(dNotchFilter *df, float val);

#endif