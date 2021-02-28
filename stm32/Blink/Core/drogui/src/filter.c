#include "filter.h"
#include "utils.h"
#include <stdlib.h>
void initFilter(filter* f, int n, float* a, float* b){
    
    initArrCoeff(&(f->arr_y),n-1, a);
    initArrCoeff(&(f->arr_u),n, b);
    f -> n = n;
}


float computeFilter(filter *f, float x){
    addArrCoeff(&(f->arr_u), x);
    float y = computeArrCoeff(&(f->arr_u)) - computeArrCoeff(&(f->arr_y));
    addArrCoeff(&(f->arr_y), y);
    return y;
}


void initArrCoeff(arrCoeff* a,int n,float * coeff){
    a->coeff = coeff;
    a->n = n;
    a->values = (float*) calloc(n, sizeof(float));
    a->head = 0;
}


float computeArrCoeff(arrCoeff* a){
    float ans = 0;
    for(int aux = (a->head-1+a->n)%(a->n), i = 0 ; i < (a->n) ; i++){
        ans += (a->coeff)[i] * (a->values[aux]);
        aux = (aux - 1 + a->n )%(a->n);
    }
    return ans;
}

void addArrCoeff(arrCoeff* a, float x){
    (a->values)[a->head++] = x;
    a->head %= a->n;
}

void initDNotchFilter(dNotchFilter* df, int n, float threshold, float fs, float a, float zeta){
    df -> n = n;
    df -> a = a;
    df -> zeta = zeta;
    df -> fs = fs;
    df -> threshold = threshold;
    
    df -> num[0]  = 1; df -> num[1] = df -> num[2] = 0;
    df -> den[0] = 0; df -> den[1] = 0;

    df -> values = (float*) calloc(n, sizeof(float));
    df -> head = 0;
    /*df -> FFT = arduinoFFT(); 
    df->exponent = (df -> FFT).Exponent(df -> n);
    
    initFilter(&(df->f), 3, df->den, df-> num);*/
}

void updateCoeffNotch(dNotchFilter *df, float fc){
    
    float wn = 2*pi*fc;
    float p = 2*(df->zeta)*wn + wn*wn;
    float ts = 1/(df->fs);
    float q = wn*wn;
    float n = wn*((df->a) +1/(df -> a));
    
    df -> num[0] = (4+p*ts*ts)/(4+2*n*ts + q*ts*ts);
    df -> num[1] = (2*p*ts*ts-8)/(4+2*n*ts + q*ts*ts);
    df -> num[2] = (4+p*ts*ts)/(4+2*n*ts + q*ts*ts);
     
    df -> den[0] = (2*q*ts*ts-8)/(4+2*n*ts + q*ts*ts);
    df -> den[1] = (4-2*n*ts+q*ts*ts)/(4+2*n*ts + q*ts*ts);
}


float computeDNotch(dNotchFilter *df, float val){

    if(df -> head < df -> n){
        df -> values[df -> head++]  = val;
    }
    else{
        for(int i = 0; i < df->n-1 ; i++)
            df-> values[i] = df-> values[i +1]; 
        df->values[ df -> n - 1] = val;
    }

    if(df -> head == df -> n){
        float fft[df->n];
        /*float imag[df->n], fft[df->n];
        for (uint16_t i = 0; i < df->n; i++) fft[i] = df->values[i], imag[i] = 0; 
        
        
        (df->FFT).Compute(fft, imag, df -> n, df->exponent, FFT_FORWARD); 
        (df->FFT).ComplexToMagnitude(fft, imag, df -> n); */
        
        
        for(int i = 0; i< df -> n ; i++) fft[i] = (i > 0 ? 2 : 1) * fft[i] / (df->n);

        int thres_ind = (df -> threshold)*(df -> n)/(df -> fs);
        float maxi = 0; float fc = 0;

        for(int i = thres_ind; i <= df -> n/2 + 1 ; i++) 
            if(fft[i] > maxi) maxi = fft[i], fc = (i)*(df->fs)/(df -> n);
        
        updateCoeffNotch(df, fc);
        
        val = computeFilter(&(df->f), val);
        df->values[df->n-1] = val;
    }       
    
    return val;    
}
