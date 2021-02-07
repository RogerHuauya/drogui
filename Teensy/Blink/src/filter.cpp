#include "../headers/filter.h"
#include <Arduino.h>


void initFilter(filter* f, int n, double a, double b){
    f -> arr = (double*) calloc(n, sizeof(double));
    f -> n = n;
    f -> a = a;
    f -> b = b;
    f -> head = f -> value = f -> value_prom = 0;
}


double computeFilter(filter *f, double x){
    
    f -> value_prom += x - (f -> arr)[f -> head];
    (f -> arr)[(f -> head)++] = x;
    f -> head %= f -> n;

    f -> value = (f ->value)*(f -> a) + (f ->value_prom)*(f -> b);
    return f -> value;
}