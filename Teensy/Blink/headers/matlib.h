#ifndef MATLIB_H
#define MATLIB_H

#include <math.h>
#include <Arduino.h>

typedef struct _mat{
    int row, col;
    float** val;
    float* aux;
} mat;

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
void matSubs(mat* ans, mat* a, mat* b);
void matInv3(mat* Rinv, mat* R);
void matTrans(mat* Rt,mat* R);
void matCopy(mat* des, mat* src);
void rpyToR(mat* R, float roll, float pitch, float yaw);
void gaussElimination3x3(mat* a, mat* b, mat* ans);
bool matInverse(mat* inverse, mat* A);

#endif
