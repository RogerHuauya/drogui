#ifndef FILTER_H
#define FILTER_H

#include "main.h"
#include "arm_math.h"

/**
 * @brief Initialize Filter Structure
 * @param f filter (arm_iir_lattice_instance_f32)
 * @param state Array of values that changes in time (float*)
*/
typedef struct _filter{
    arm_iir_lattice_instance_f32 f;
    float *state;
}filter ;

/**
 * @brief Initialize Dynamic Notch Filter Structure
 * @param values Array of values that changes in time (float*)
 * @param n windows size (int)
 * @param fs Sampling frecuency (float)
 * @param a Gain (float)
 * @param threshold(float)
 * @param coeffs Coeficients of biqud cascade filter (float)
 * @param state State array of filter (float)
 * @param head(int)  
 * @param exponent
 * @param fft Fast Fourier Transform (arm_rfft_fast_instance_f32)
 * @param f Biquad Filter (arm_biquad_cascade_df2T_instance_f32)
 */
typedef struct _dNotchFilter{
    float *values;
    int n;
    float fs, threshold, a, zeta;
    float coeffs[5];
    float state[4];
    int head;
    uint8_t exponent;
    arm_rfft_fast_instance_f32 fft;
    arm_biquad_cascade_df2T_instance_f32 f;
}dNotchFilter;

/**
 * @brief Initialize filter 
 * @param f PID (struct filter)
 * @param n Number of stages in the filter (float)
 * @param k  Array of K (float*)
 * @param v Array of V (float*)
*/

void initFilter(filter* f, int n, float* k, float* v);
/**
 * @brief Get the current filter signal 
 * @param f filter (struct filter)
 * @param x Current value of the signal (float)
*/
float computeFilter(filter *f, float x);
/**
 * @brief Initialize Dynamic Notch Filter 
 * @param df Dynamic Notch filter (struct dNotchFilter)
 * @param n windows size (int)
 * @param fs Sampling frecuency (float)
 * @param a Gain (float)
 * @param threshold(float)
 */
void initDNotchFilter(dNotchFilter* df, int n, float threshold, float fs, float a, float zeta);
/**
 * @brief Get the current filter signal 
 * @param df Dynamic Notch filter (struct dNotchFilter)
 * @param val Current value of the signal (float)
*/
float computeDNotch(dNotchFilter *df, float val);

#endif