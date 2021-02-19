#include "../headers/filter.h"
#include <Arduino.h>


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