#ifndef FILTER_H
#define FILTER_H

#include "main.h"
#include "arm_math.h"

/**
 * @brief Initialize Filter Structure
 * @param f filter (arm_iir_lattice_instance_f32)
 * @param n Number of stages in the filter (float)
 * @param state Array of values that changes in time (float*)
*/
typedef struct _filter{
    arm_iir_lattice_instance_f32 f;
    int n;
    float *state;
}filter ;

/**
 * @brief Initialize filter 
 * @param f Filter (pointer filter)
 * @param n Number of stages in the filter (float)
 * @param k  Array of K (float*)
 * @param v Array of V (float*)
*/
void initFilter(filter* f, int n, float* k, float* v);

/**
 * @brief Get the current filter signal 
 * @param f filter (pointer filter)
 * @param x Current value of the signal (float)
*/
float computeFilter(filter *f, float x);

/**
 * @brief Clean filter  
 * @param f filter (pointer filter)
*/
void cleanFilter(filter *f);

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
    arm_biquad_casd_df1_inst_f32 f;
}dNotchFilter;

/**
 * @brief Initialize Dynamic Notch Filter 
 * @param df Dynamic Notch filter (pointer dNotchFilter)
 * @param n windows size (int)
 * @param fs Sampling frecuency (float)
 * @param a Gain (float)
 * @param threshold(float)
 */
void initDNotchFilter(dNotchFilter* df, int n, float threshold, float fs, float a, float zeta);

/**
 * @brief Get the current filter signal 
 * @param df Dynamic Notch filter (pointer dNotchFilter)
 * @param val Current value of the signal (float)
*/
float computeDNotch(dNotchFilter *df, float val);

/**
 * @brief Clean Dynamic Notch filter  
 * @param df Dynamic Notch filter (pointer dNotchFilter)
*/
void cleanDNotch(dNotchFilter *df);

/**
 * @brief Ema Filter Struct ( a + b = 1)
 * @param a Coefficient (float) 
 * @param b Coefficient (float)
 * @param threshold (float)  
 * @param value Filtered value (float)
*/
typedef struct _emaFilter{
    float a, b, threshold;
    float value;
} emaFilter;

/**
 * @brief Initialize Ema Filter
 * @param ef Ema Filter(pointer emmaFilter)
 * @param a Coefficient (float)
 * @param b Coefficient (float)
 * @param threshold (float)  
*/
void initEmaFilter(emaFilter* ef, float a, float b, float threshold);

/**
 * @brief Compute Ema Filter
 * @param ef Ema Filter(pointer emmaFilter)
 * @param val Current value (float)
 * @return Filtered value (float)
*/
float computeEmaFilter(emaFilter* ef, float val);

/**
 * @brief Average Filter Struct
 * @param n Size of array (int)
 * @param head (int)
 * @param values Array of values(float*)
 * @param sum Acumulated value (float)
*/
typedef struct _mvAvgFilter{
    int n, head;
    float* values;
    float sum;
} mvAvgFilter;

/**
 * @brief Initialize Average Filter
 * @param mf Average Filter(pointer mvAvgFilter)
 * @param n  Number of values (int)
*/
void initMvAvgFilter(mvAvgFilter* mf, int n);

/**
 * @brief Compute Average Filter
 * @param mf Filter(pointer mvAvgFilter)
 * @param val Current value (float)
 * @return Average value (float)
*/
float compueteMvAvgFilter(mvAvgFilter* mf, float val);

#endif