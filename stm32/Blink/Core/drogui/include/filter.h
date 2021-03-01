#ifndef FILTER_H
#define FILTER_H

#include "main.h"
#include "arm_math.h"


typedef struct _filter{
    arm_iir_lattice_instance_f32 f;
    float *state;
}filter ;

typedef struct _dNotchFilter{
    float *values;
    int n;
    float fs, threshold, a, zeta;
    float coeffs[5];
    float state[4];
    int head;
    uint8_t exponent;
    arm_cfft_radix2_instance_f32 fft;
    arm_biquad_cascade_df2T_instance_f32 f;
}dNotchFilter;

void initFilter(filter* f, int n, float* a, float* b);
float computeFilter(filter *f, float x);


void initDNotchFilter(dNotchFilter* df, int n, float threshold, float fs, float a, float zeta);
float computeDNotch(dNotchFilter *df, float val);

#endif