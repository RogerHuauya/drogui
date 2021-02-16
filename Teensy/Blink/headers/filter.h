#ifndef FILTER_H
#define FILTER_H


struct arrCoeff{
    int n, head;
    float *values, *coeff;
};

struct filter{
    int n;
    arrCoeff arr_u, arr_y;
};

void initFilter(filter* f, int n, float* a, float* b);
double computeFilter(filter *f, float x);

void initArrCoeff(arrCoeff* a, int n, float * coeff);
float computeArrCoeff(arrCoeff* a);
void addArrCoeff(arrCoeff* a, float x);


#endif