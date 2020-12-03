#include "matlib.h"
#include "serial.h"
extern char s[50];
extern serial Serial1;
void matInit(mat* m, int row, int col){
    m->row = row;
    m->col = col;


    m->val = (fractional**) malloc(row* sizeof(fractional*));

	m-> aux = malloc(row*col* sizeof(fractional));
    for (int i = 0; i < row; i++) {
            m->val[i] = m-> aux + i*col;
    }
}


void matMult(mat* ans, mat* a, mat* b){
    if(a->col == b->row){
        MatrixMultiply(a->row, a->col, b->col, &ans->val[0][0], &a->val[0][0], &b->val[0][0]);
    }
    return;
}
void matScale(mat* ans, float alpha){
    MatrixScale(ans->row, ans->col, ans->val, ans->val, Float2Fract(alpha));
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
    for(int i = 0; i < n; i++)  m->val[i][i] = Float2Fract(1 - 1.0/(1<<15));
}

double q2double(int q){
    return  q/32768.0;
}

void setMatVal(mat* m, int i, int j, float value){
    m->val[i][j] = Float2Fract(value);
}

float getMatVal(mat* m, int i, int j){
    return Fract2Float(m->val[i][j]);
}

void matDestruct(mat* m){
    free(m->val);
    free(m->aux);
}