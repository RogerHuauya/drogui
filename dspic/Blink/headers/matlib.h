#ifndef MATLIB_H
#define MATLIB_H
#include <dsp.h>
typedef struct _mat
{
    int row, col;
    fractional val[6][6];
} mat;
void matInit(mat* m, int row, int col);
void matMult(mat* ans, mat* a, mat* b);
void quaternionToR(mat* R, float q1, float q2, float q3, float q4);
double q2double(int q);
int double2q(double d);
void setMatVal(mat* m, int i, int j, float value);
float getMatVal(mat* m, int i, int j);
#endif