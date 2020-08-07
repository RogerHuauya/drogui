#include "filter.h"

void initFilter(filter* f, double alpha, int n , int m){
    f -> n = n, f -> m = m;
    f -> heady = f -> headu = 0;

    f -> alp = alpha;
    f -> a = (double *)malloc( m *sizeof(double));
    f -> b = (double *)malloc( n *sizeof(double));
    f -> ubuff = (double *)calloc( m, sizeof(double));
    f -> ybuff = (double *)calloc( n, sizeof(double));
    *f -> a = alpha, *f->b = 1 - alpha;
}

double computeFilter(filter* f, double u){
    double y = 0;
    int i = 0;
    f -> headu = (f->headu + f->m - 1)%f->m;
    f -> ubuff[f->headu] = u;
    for(i = 0; i < f->n; i++) 
        y += f->b[i]*(f->ybuff[(f->heady + i)%f->n]);
    
    for(i = 0; i < f->m; i++) 
        y += f->a[i]*(f->ubuff[(f->headu + i)%f->m]);
    
    f -> heady = (f -> heady + f -> n - 1)% f-> n;
    f -> ybuff[f -> heady] = y;
    return y;
}