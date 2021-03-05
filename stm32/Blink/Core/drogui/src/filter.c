#include "filter.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <usart.h>
#include <string.h>


void initFilter(filter* f, int n, float* k, float* v){
    f->state = (float*) calloc(n, sizeof(float));
    arm_iir_lattice_init_f32(&(f->f), n, k, v, f->state, 1);
}


float computeFilter(filter *f, float x){
    float ans;
    arm_iir_lattice_f32(&(f->f), &x, &ans, 1);
    return ans;
}




void initDNotchFilter(dNotchFilter* df, int n, float threshold, float fs, float a, float zeta){
    df -> n = n;
    df -> a = a;
    df -> zeta = zeta;
    df -> fs = fs;
    df -> threshold = threshold;
    
    df -> coeffs[0] = 1;
    df -> coeffs[1] = df -> coeffs[2] = df -> coeffs[3] = df -> coeffs[4] = 0;
    df -> state[0] = df -> state[1] = df -> state[2] = df -> state[3] =  0;

    df -> values = (float*) calloc(n, sizeof(float));
    df -> head = 0;
    arm_rfft_fast_init_f32(&(df->fft), n);
    arm_biquad_cascade_df2T_init_f32(&(df->f), 1, df->coeffs, df->state);
}

void updateCoeffNotch(dNotchFilter *df, float fc){
    
    float wn = 2*pi*fc;
    float p = 2*(df->zeta)*wn + wn*wn;
    float ts = 1/(df->fs);
    float q = wn*wn;
    float n = wn*((df->a) +1/(df -> a));
    
    df -> coeffs[0] = (4+p*ts*ts)/(4+2*n*ts + q*ts*ts);
    df -> coeffs[1] = (2*p*ts*ts-8)/(4+2*n*ts + q*ts*ts);
    df -> coeffs[2] = (4+p*ts*ts)/(4+2*n*ts + q*ts*ts);
     
    df -> coeffs[3] = -(2*q*ts*ts-8)/(4+2*n*ts + q*ts*ts);
    df -> coeffs[4] = -(4-2*n*ts+q*ts*ts)/(4+2*n*ts + q*ts*ts);
}


float computeDNotch(dNotchFilter *df, float val){

    float ans;
    arm_biquad_cascade_df2T_f32(&(df ->f), &val, &ans, 1);

    df -> values[df -> head++]  = val;
    if(df->head == df->n){

        float fft_arr[df->n];
        float fft_ans[df->n];
        for (uint16_t i = 0; i < (df->n) ; i++) fft_arr[i] = df->values[i]; 
        
        arm_rfft_fast_f32(&(df->fft), fft_arr, fft_ans, 0);
        
        for(int i = 0; i< (df -> n) ; i += 2) 
            arm_sqrt_f32(fft_ans[i]*fft_ans[i] + fft_ans[i+1]*fft_ans[i+1], &fft_ans[i/2]);
        
        for(int i = 0; i < (df -> n)/2 ; i ++) 
            fft_ans[i] = (i > 0 ? 2 : 1) * fft_ans[i] / (df->n);

        int thres_ind = (df -> threshold)*(df -> n)/(df -> fs);
        float maxi = 0; float fc = 0;

        for(int i = thres_ind; i < (df -> n)/2 ; i ++) 
            if(fft_ans[i] > maxi) maxi = fft_ans[i], fc = (i)*(df->fs)/(df -> n);
        
        updateCoeffNotch(df, fc);
        df->head = 0;
    }   
    return ans;    
}
