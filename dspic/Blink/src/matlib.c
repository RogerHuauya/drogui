#include "matlib.h"
void matMult(mat* ans, mat* a, mat* b){
    if(a->col == b->row){
        for(int i = 0; i < a->row;i++)
            for(int j = 0; j < b->col;j++){
                ans->val[i][j] = 0;
                for(int k = 0; k < a->col; k++){
                    ans->val[i][j] += a->val[i][k]*b->val[k][j];
                }
            }
    }
    return;
}

void quaternionToR(mat* R, float q1, float q2, float q3, float q4){
    R->row = R->col = 3;
    R->val[0][0] = 2*(q1*q1 + q2*q2) - 1;
    R->val[0][1] = 2*(q2*q3 - q1*q4);
    R->val[0][2] = 2*(q2*q4 + q1*q4);
    R->val[1][0] = 2*(q2*q3 + q1*q4);
    R->val[1][1] = 2*(q1*q1 + q3*q3) - 1;
    R->val[1][2] = 2*(q3*q4 - q1*q2);
    R->val[2][0] = 2*(q2*q4 - q1*q3);
    R->val[2][1] = 2*(q3*q4 + q1*q2);
    R->val[2][2] = 2*(q1*q1 + q4*q4) - 1;
}