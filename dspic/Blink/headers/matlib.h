#ifndef MATLIB_H
#define MATLIB_H
typedef struct _mat
{
    int row, col;
    float val[5][5];
} mat;
void matMult(mat* ans, mat* a, mat* b);
#endif