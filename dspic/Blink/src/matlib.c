#include "matlib.h"
#include "serial.h"
extern char s[50];
extern serial Serial1;

#if CASO == 1
void matInit(mat* m, int row, int col){
    m->row = row;
    m->col = col;
    
    m->val = (fractional**) calloc(row, sizeof(fractional*));

	m-> aux = calloc(row*col, sizeof(fractional));
    for (int i = 0; i < row; i++) {
            m->val[i] = m-> aux + i*col;
    }
}

void matMult(mat* ans, mat* a, mat* b){
    if(a->col == b->row){
        MatrixMultiply(a->row, a->col, b->col, &ans->val[0][0], &a->val[0][0], &b->val[0][0]);
    }
    //matScale(ans, ans, SCALE);
    return;
}
void matScale(mat* ans, mat* a, float alpha){
    MatrixScale(ans->row, ans->col, &ans->val[0][0], &a->val[0][0], Float2Fract(alpha));
}

void matAdd(mat* ans, mat* a, mat* b){
    MatrixAdd(ans->row, ans->col, &ans->val[0][0], &a->val[0][0], &b->val[0][0]);
}
void quaternionToR(mat* R, float n, float ex, float ey, float ez){
    R->row = R->col = 3;
    
    R->val[0][0] = Float2Fract(2*(n*n + ex*ex) - 1);
    R->val[0][1] = Float2Fract(2*(ex*ey - n*ez));
    R->val[0][2] = Float2Fract(2*(ex*ez + n*ey));

    R->val[1][0] = Float2Fract(2*(ex*ey + n*ez));
    R->val[1][1] = Float2Fract(2*(n*n + ey*ey) - 1);
    R->val[1][2] = Float2Fract(2*(ey*ez - n*ex));
    
    R->val[2][0] = Float2Fract(2*(ex*ez - n*ey));
    R->val[2][1] = Float2Fract(2*(ey*ez + n*ex));
    R->val[2][2] = Float2Fract(2*(n*n + ez*ez) - 1);
}


void eye(mat* m, int n){
    matInit(m, n, n);
    for(int i = 0; i < n; i++)  m->val[i][i] = Float2Fract((1 - 1.0/(1<<15)));
}

void setMatVal(mat* m, int i, int j, float value){
    m->val[i][j] = Float2Fract(value/SCALE);
}

float getMatVal(mat* m, int i, int j){
    return Fract2Float(m->val[i][j])*SCALE;
}

void matDestruct(mat* m){
    free(m->val);
    free(m->aux);
}
#else

void matInit(mat* m, int row, int col){
    m->row = row;
    m->col = col;
    
    m->val = (double**) calloc(row, sizeof(double*));

	m-> aux = calloc(row*col, sizeof(double));
    for (int i = 0; i < row; i++) {
            m->val[i] = m-> aux + i*col;
    }
}

void matMult(mat* ans, mat* a, mat* b){
    for(int i = 0 ;i < a->row; i++)
        for(int j = 0 ; j< b->col ; j++){
            double aux = 0;
            for(int k = 0 ; k < a-> col; k++)
                aux += a->val[i][k]* b->val[k][j];
            ans->val[i][j] = aux;
        }
}

void matScale(mat* ans, mat* a, float alpha){
    for(int i = 0 ;i < a->row; i++)
        for(int j = 0 ; j< a->col ; j++){
            ans ->val[i][j] = a->val[i][j]*alpha;
        }
}

void matAdd(mat* ans, mat* a, mat* b){
    for(int i = 0 ;i < ans->row; i++)
        for(int j = 0 ; j< ans->col ; j++){
            ans ->val[i][j] = a->val[i][j] + b->val[i][j];
        }
}

void matSubs(mat* ans, mat* a, mat* b){
    for(int i = 0 ;i < ans->row; i++)
        for(int j = 0 ; j< ans->col ; j++){
            ans ->val[i][j] = a->val[i][j] - b->val[i][j];
        }
}

void matTrans(mat* Rt,mat* R){
    for( int i = 0; i < R->row; i++ ){
        for( int j = 0; j < R->col; j++ ){
            Rt->val[j][i] = R->val[i][j];
        }
    }
}
double det3(mat* R){
    double det = ((R->val[0][0])*(R->val[1][1])*(R->val[2][2])) +((R->val[0][1])*(R->val[1][2])*(R->val[2][0]))
     + ((R->val[0][2])*(R->val[1][0])*(R->val[2][1])) - ((R->val[0][2])*(R->val[1][1])*(R->val[2][0])) 
     - ((R->val[0][1])*(R->val[1][0])*(R->val[2][2])) - ((R->val[0][0])*(R->val[1][2])*(R->val[2][1]));
    
    return det;
}
void matInv3(mat* Rinv, mat* R){
    double val_det = 0;
    mat aux1;

    val_det = det3(R); 
    
    matInit(&aux1,3,3);
    
    matTrans(&aux1,R);
    
    for( int i = 0; i < 3; i++ ){
        for( int j = 0; j < 3; j++ ){
            Rinv->val[i][j] = ((aux1.val[(i+1)%3][(j+1)%3])*(aux1.val[(i+2)%3][(j+2)%3]) - (aux1.val[(i+1)%3][(j+2)%3])*(aux1.val[(i+2)%3][(j+1)%3]) );
            Rinv->val[i][j] = 1.0*(1/val_det)*(Rinv->val[i][j]);
            //Rinv->val[i][j] = aux1.val[i][j];
        }
    }
    matDestruct(&aux1);
}

void quaternionToR(mat* R, float n, float ex, float ey, float ez){
    R->row = R->col = 3;
    
    Rq(0+1, 0+1) = 2*(n*n + ex*ex) - 1;
    Rq(0+1, 1+1) = 2*(ex*ey - n*ez);
    Rq(0+1, 2+1) = 2*(ex*ez + n*ey);

    Rq(1+1, 0+1) = 2*(ex*ey + n*ez);
    Rq(1+1, 1+1) = 2*(n*n + ey*ey) - 1;
    Rq(1+1, 2+1) = 2*(ey*ez - n*ex);
    
    Rq(2+1, 0+1) = 2*(ex*ez - n*ey);
    Rq(2+1, 1+1) = 2*(ey*ez + n*ex);
    Rq(2+1, 2+1) = 2*(n*n + ez*ez) - 1;
}

void eye(mat* m, int n){
    matInit(m, n, n);
    for(int i = 0; i < n; i++)  m->val[i][i] = 1;
}

void setMatVal(mat* m, int i, int j, float value){
    m->val[i][j] = value;
}

float getMatVal(mat* m, int i, int j){
    return m->val[i][j];
}
void matCopy(mat* des, mat* src){
    for(int i = 0 ; i < des->row ; i++)
        for(int j = 0 ; j < des->col ; j++)
            des->val[i][j] = src->val[i][j];
}
void matDestruct(mat* m){
    free(m->val);
    free(m->aux);
}

#endif
