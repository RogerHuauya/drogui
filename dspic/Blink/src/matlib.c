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