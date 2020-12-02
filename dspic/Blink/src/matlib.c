#include "matlib.h"
void matInit(mat* m, int row, int col){
    m->row = row;
    m->col = col;
    for(int i = 0; i < m->row; i++)
        for(int j = 0; j < m->col; j++)
            m->val[i][j] = Float2Fract(0.0);
}
void matMult(mat* ans, mat* a, mat* b){
    if(a->col == b->row){
        MatrixMultiply(a->row, a->col, b->col, &ans->val[0][0], &a->val[0][0], &b->val[0][0]);
    }
    return;
}

void quaternionToR(mat* R, float q1, float q2, float q3, float q4){
    R->row = R->col = 3;
    R->val[0][0] = Float2Fract(2*(q1*q1 + q2*q2) - 1);
    R->val[0][1] = Float2Fract(2*(q2*q3 - q1*q4));
    R->val[0][2] = Float2Fract(2*(q2*q4 + q1*q4));
    R->val[1][0] = Float2Fract(2*(q2*q3 + q1*q4));
    R->val[1][1] = Float2Fract(2*(q1*q1 + q3*q3) - 1);
    R->val[1][2] = Float2Fract(2*(q3*q4 - q1*q2));
    R->val[2][0] = Float2Fract(2*(q2*q4 - q1*q3));
    R->val[2][1] = Float2Fract(2*(q3*q4 + q1*q2));
    R->val[2][2] = Float2Fract(2*(q1*q1 + q4*q4) - 1);
}

void eye(mat* m, int n){
    matInit(m, n, n);
    for(int i = 0; i < n; i++)  m->val[i][i] = Float2Fract(0.999);
}

double q2double(int q){
    return  q/32768.0;
}

void setMatVal(mat* m, int i, int j, float value){
    m->val[i][j] = Float2Fract(value);
}

int double2q(double d){
    return  d*32768.0;
}