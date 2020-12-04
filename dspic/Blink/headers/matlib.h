#ifndef MATLIB_H
#define MATLIB_H
#include <dsp.h>

#define SCALE 64.0

#define CASO 0
#if CASO == 1

typedef struct _mat{
    int row, col;
    fractional** val;
    fractional* aux;
    double **vald;
} mat;

#else

typedef struct _mat{
    int row, col;
    double** val;
    double* aux;
} mat;

#endif

void matInit(mat* m, int row, int col);
void matMult(mat* ans, mat* a, mat* b);
void quaternionToR(mat* R, float q1, float q2, float q3, float q4);
double q2double(int q);
int double2q(double d);
void setMatVal(mat* m, int i, int j, float value);
float getMatVal(mat* m, int i, int j);
void matDestruct(mat* m);
void matScale(mat* ans, mat* a, float alpha);
void matAdd(mat* ans, mat* a, mat* b);
#endif