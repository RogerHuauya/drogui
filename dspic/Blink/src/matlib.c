#include "matlib.h"

void matInit(mat* m, int row, int col){
    m->row = row;
    m->col = col;
    m->val = (fractional*) calloc(row*col,sizeof(fractional));
}


void matMult(mat* ans, mat* a, mat* b){
    if(a->col == b->row){
        MatrixMultiply(a->row, a->col, b->col, ans->val, a->val, b->val);
    }
    return;
}

void quaternionToR(mat* R, float q1, float q2, float q3, float q4){
    R->row = R->col = 3;
    
    *((R->val) + 0*(R->row) + 0 ) = Float2Fract(2*(q1*q1 + q2*q2) - 1);
    *((R->val) + 0*(R->row) + 1 ) = Float2Fract(2*(q2*q3 - q1*q4));
    *((R->val) + 0*(R->row) + 2 ) = Float2Fract(2*(q2*q4 + q1*q4));
    *((R->val) + 1*(R->row) + 0 ) = Float2Fract(2*(q2*q3 + q1*q4));
    *((R->val) + 1*(R->row) + 1 ) = Float2Fract(2*(q1*q1 + q3*q3) - 1);
    *((R->val) + 1*(R->row) + 2 ) = Float2Fract(2*(q3*q4 - q1*q2));
    *((R->val) + 2*(R->row) + 0 ) = Float2Fract(2*(q2*q4 - q1*q3));
    *((R->val) + 2*(R->row) + 1 ) = Float2Fract(2*(q3*q4 + q1*q2));
    *((R->val) + 2*(R->row) + 2 ) = Float2Fract(2*(q1*q1 + q4*q4) - 1);
}

void eye(mat* m, int n){
    matInit(m, n, n);
    for(int i = 0; i < n; i++)  *((m->val) +i*(m->row) + i) = Float2Fract(1 - 1.0/(1<<15));
}

double q2double(int q){
    return  q/32768.0;
}

void setMatVal(mat* m, int i, int j, float value){
    *((m->val) +i*(m->row) + j) = Float2Fract(value);
}

float getMatVal(mat* m, int i, int j){
    return Fract2Float(*((m->val) +i*(m->row) + j));
}


int double2q(double d){
    return  d*32768.0;
}