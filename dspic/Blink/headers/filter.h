#ifndef FILTER_H
#define FILTER_H

#include<stdlib.h>
typedef struct _filter{
    /*  Generic Discrete Filter transfer function 
                            -1       -2
        Y(Z) = b[0] + b[1]*z + b[2]*z + ...
        ----   ------------------------------
                          -1       -2
        X(Z)    1 + a[0]*z + a[1]*z  + ...     */
	double alp, *a, *b, *ybuff, *ubuff;
    int n,  m , heady, headu;
} filter;

void initFilter(filter* f, double alpha, int n , int m);

double computeFilter(filter* f, double u);

#endif