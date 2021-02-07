#ifndef FILTER_H
#define FILTER_H

struct filter{
    double* arr;
    int n, head;
    double a, b, value, value_prom;
};

void initFilter(filter* f, int n, double a, double b);
double computeFilter(filter *f, double x);


#endif