#ifndef FILTER_H
#define FILTER_H


struct arrCoeff{
    int n, head;
    double *values, *coeff;
};

struct filter{
    int n;
    arrCoeff arr_u, arr_y;
};

void initFilter(filter* f, int n, double* a, double* b);
double computeFilter(filter *f, double x);

void initArrCoeff(arrCoeff* a, int n, double * coeff);
double computeArrCoeff(arrCoeff* a);
void addArrCoeff(arrCoeff* a, double x);


#endif