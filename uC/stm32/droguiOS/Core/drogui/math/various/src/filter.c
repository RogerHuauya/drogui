#include "filter.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <usart.h>
#include <string.h>
#include "sensorsTasks.h"
float fc;

void initFilter(filter* f, int n, float* k, float* v){
    f->n = n;
    f->state = (float*) calloc(n, sizeof(float));
    arm_iir_lattice_init_f32(&(f->f), f->n, k, v, f->state, 1);
}


float computeFilter(filter *f, float x){
    float ans;
    arm_iir_lattice_f32(&(f->f), &x, &ans, 1);
    return ans;
}

void cleanFilter(filter *f){
    for( int i = 0; i < (f->n); i++ ) f->state[i] = 0;
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
    arm_biquad_cascade_df1_init_f32(&(df->f), 1, df->coeffs, df->state);
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

void cleanDNotch(dNotchFilter *df){

    for( int i = 0; i < df->n; i++ ) df->values[i] = 0;

    df -> coeffs[0] = 1;
    df -> coeffs[1] = df -> coeffs[2] = df -> coeffs[3] = df -> coeffs[4] = 0;
    df -> state[0] = df -> state[1] = df -> state[2] = df -> state[3] =  0;

}

float computeDNotch(dNotchFilter *df, float val){


    float ans;

    arm_biquad_cascade_df1_f32(&(df ->f), &val, &ans, 1);

    val = ans;

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

        int thres_ind = round((df -> threshold)*(df -> n)/(df -> fs));
        float maxi = 0; fc = 0;

        for(int i = thres_ind; i < (df -> n)/2 ; i ++)
            if(fft_ans[i] > maxi) maxi = fft_ans[i], fc = (i)*(df->fs)/(df -> n);



        updateCoeffNotch(df, fc);
        df->head = 0;
    }

    return ans;
}




void initEmaFilter(emaFilter* ef, float a, float b, float threshold){
    ef -> a = a;
    ef -> b = b;
    ef -> threshold = threshold;
    ef -> value = 0;
}

float computeEmaFilter(emaFilter* ef, float val){
    float ans = 0;
    ans = (ef -> value) * (ef -> a) + val * (ef -> b);

    float diff = val - ans;
    diff = fmax(fmin(diff, 1), -1);

    if (fabs(diff) >  (ef->threshold)) ans += diff / 6.0;
    return (ef->value) = ans;
}


void initMvAvgFilter(mvAvgFilter* mf, int n){
    mf -> values = (float *) calloc(n, sizeof(float));
    mf -> n = n;
    mf -> head = 0;
    mf -> sum = 0;
}

float computeMvAvgFilter(mvAvgFilter* mf, float val){
    mf -> sum -= mf->values[(mf -> head)];
    mf -> values[(mf -> head)] = val;
    mf -> sum += mf -> values[ (mf -> head) ++];
    mf -> head = (mf -> head)%(mf -> n);
    return (float) (mf -> sum)/(mf->n);
}


void initSavGolDFilter(savGolDFilter* sg, int n){
    sg->n = n;
    if(n == 7)
        sg->coeffs = SG7;
    else if( n == 10)
        sg->coeffs = SG10;
    else
        sg->coeffs = SG13;
    sg->head = 0;
    sg->values = (float *) calloc(n, sizeof(float));
}

float computeSavGolDFilter(savGolDFilter* sg, float value){
    float ret = 0;
    sg->values[sg->head] = value;

    for(int i = 0; i < sg->n; i++)
        ret += sg->values[(sg->head -i + sg->n)%(sg->n)] * sg->coeffs[i];

    sg->head = (sg->head + 1)%(sg->n);

    return ret;
}





