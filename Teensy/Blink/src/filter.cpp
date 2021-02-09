#include "../headers/filter.h"
#include <Arduino.h>


void initFilter(filter* f, int n, double* a, double* b){
    initArrCoeff(&(f->arr_y),n-1, a);
    initArrCoeff(&(f->arr_u),n, b);
    f -> n = n;
}


double computeFilter(filter *f, double x){
    addArrCoeff(&(f->arr_u), x);
    double y = computeArrCoeff(&(f->arr_u)) - computeArrCoeff(&(f->arr_y));
    addArrCoeff(&(f->arr_y), y);
    return y;
}


void initArrCoeff(arrCoeff* a,int n,double * coeff){
    a->coeff = coeff;
    a->n = n;
    a->values = (double*) calloc(n, sizeof(double));
    a->head = 0;
}


double computeArrCoeff(arrCoeff* a){
    double ans = 0;
    for(int aux = (a->head-1+a->n)%(a->n), i = 0 ; i < (a->n) ; i++){
        ans += (a->coeff)[i] * (a->values[aux]);
        aux = (aux - 1 + a->n )%(a->n);
    }
    return ans;
}

void addArrCoeff(arrCoeff* a, double x){
    (a->values)[a->head++] = x;
    a->head %= a->n;
}