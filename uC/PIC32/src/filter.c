#include "filter.h"

void initFilter(filter* f, double alpha){
    f -> n = 1, f -> m = 1;
    f -> heady = f -> headu = 0;

    f -> alp = alpha;
    int i = 0;
    for(i = 0 ; i< f->n; i++) f->a[i] = f->ybuff[i] = 0;
    for(i = 0 ; i< f->m; i++) f->b[i] = f->ubuff[i] = 0;

    *f -> a = alpha;
    *f -> b = 1 - alpha;
}

double  computeFilter(filter* f, double u){
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