#include "matlib.h"
#include <stdbool.h>


void getCofactor(mat* A, mat* temp, int p, int q);
void matInit(mat* m, int row, int col){
    m->row = row;
    m->col = col;
	m->val = (float*) calloc(row*col, sizeof(float));
    arm_mat_init_f32(&(m->M), row, col, m->val);
}

void matMult(mat* ans, mat* a, mat* b){
    arm_mat_mult_f32(&(a->M), &(b->M), &(ans->M));
}

void matScale(mat* ans, mat* a, float alpha){
    arm_mat_scale_f32(&(a->M), alpha, &(ans->M));
}

void matAdd(mat* ans, mat* a, mat* b){
    arm_mat_add_f32(&(a->M), &(b->M), &(ans->M));
}

void matSubs(mat* ans, mat* a, mat* b){
    arm_mat_sub_f32(&(a->M), &(b->M), &(ans->M));
}

void matTrans(mat* ans,mat* a){
    arm_mat_trans_f32(&(a->M), &(ans->M));	
}


float det3(mat* R){
    float det = + ((R->val[0*(R->col) + 0]) * (R->val[1*(R->col) + 1]) * (R->val[2*(R->col) + 2])) 
                + ((R->val[0*(R->col) + 1]) * (R->val[1*(R->col) + 2]) * (R->val[2*(R->col) + 0]))
                + ((R->val[0*(R->col) + 2]) * (R->val[1*(R->col) + 0]) * (R->val[2*(R->col) + 1])) 
                - ((R->val[0*(R->col) + 2]) * (R->val[1*(R->col) + 1]) * (R->val[2*(R->col) + 0])) 
                - ((R->val[0*(R->col) + 1]) * (R->val[1*(R->col) + 0]) * (R->val[2*(R->col) + 2]))
                - ((R->val[0*(R->col) + 0]) * (R->val[1*(R->col) + 2]) * (R->val[2*(R->col) + 1]));
    
    return det;
}


void matInv(mat* ans, mat* a){
    mat aux;
	matInit(&aux, a->row, a->col);
	matCopy(&aux, a);
    arm_mat_inverse_f32	(&(aux.M), &(ans->M));
	matDestruct(&aux);
}

void quaternionToR(mat* R, float n, float ex, float ey, float ez){
    R->row = R->col = 3;
    
    R->val[0*(R->col)+0] = 2*(n*n + ex*ex) - 1;
    R->val[0*(R->col)+1] = 2*(ex*ey - n*ez);
    R->val[0*(R->col)+2] = 2*(ex*ez + n*ey);

    R->val[1*(R->col)+0] = 2*(ex*ey + n*ez);
    R->val[1*(R->col)+1] = 2*(n*n + ey*ey) - 1;
    R->val[1*(R->col)+2] = 2*(ey*ez - n*ex);
    
    R->val[2*(R->col)+0] = 2*(ex*ez - n*ey);
    R->val[2*(R->col)+1] = 2*(ey*ez + n*ex);
    R->val[2*(R->col)+2] = 2*(n*n + ez*ez) - 1;
}

void rpyToR(mat* R, float roll, float pitch, float yaw){
    R->row = R->col = 3;
    
    R->val[0*(R->col)+0] = cos(yaw)*cos(pitch);
    R->val[0*(R->col)+1] = cos(yaw)*sin(pitch)*sin(roll) - sin(yaw)*cos(roll);
    R->val[0*(R->col)+2] = cos(yaw)*sin(pitch)*cos(roll) + sin(yaw)*sin(roll);

    R->val[1*(R->col)+0] = sin(yaw)*cos(pitch);
    R->val[1*(R->col)+1] = sin(yaw)*sin(pitch)*sin(roll) + cos(yaw)*cos(roll);
    R->val[1*(R->col)+2] = sin(yaw)*sin(pitch)*cos(roll) - cos(yaw)*sin(roll);
    
    R->val[2*(R->col)+0] = -sin(pitch);
    R->val[2*(R->col)+1] = cos(pitch)*sin(roll);
    R->val[2*(R->col)+2] = cos(pitch)*cos(roll);
}

void eye(mat* m, int n){
    matInit(m, n, n);
    for(int i = 0; i < n; i++)  m->val[i*(m->col) + i] = 1;
}

void setMatVal(mat* m, int i, int j, float value){
    m->val[i*(m->col) + j] = value;
}

float getMatVal(mat* m, int i, int j){
    return m->val[i*m->col + j];
}

void matCopy(mat* des, mat* src){
    for(int i = 0 ; i < des->row ; i++)
        for(int j = 0 ; j < des->col ; j++)
            des->val[i*(des->col) + j] = src->val[i*(src->col) + j];
}

void matDestruct(mat* m){
    free(m->val);
}

void gaussElimination3x3(mat* a, mat* b, mat* ans){
    float c;
    for(int j = 0; j < a->row; j++)
        for(int i = 0; i < a->row; i++){
            if(i!=j){
                c = getMatVal(a, i, j)/getMatVal(a, j, j);
                for(int k = 0; k < a->row; k++) a->val[i*(a->col)+k] = a->val[i*(a->col)+k] - c*a->val[j*(a->col)+k];
                b->val[i*(b->col)+0] = b->val[i*(b->col)+0] - c*b->val[j*(b->col)+0];
            }
        }
    for(int i = 0; i < a->row; i++) ans->val[i*(ans->col)+0] = b->val[i*(b->col)+0]/a->val[i*(a->col)+i];
}

float matDet(mat* A){

    float D = 0; 
    int n = 0;
    if(A->row == A->col) n = A->row;
    if (A->row == 1) 
        return getMatVal(A, 0, 0); 
  
    mat temp; // To store cofactors 
    matInit(&temp, A->row-1, A->row -1 );
    int sign = 1;  // To store sign multiplier 
  
     // Iterate for each element of first row 
    for (int f = 0; f < n; f++) 
    { 
        // Getting Cofactor of A[0][f] 
        getCofactor(A, &temp, 0, f); 
        D += sign * getMatVal(A, 0, f)*matDet(&temp); 
  
        // terms are to be added with alternate sign 
        sign = -sign; 
    } 
    matDestruct(&temp);
    return D; 
} 

void getCofactor(mat* A, mat* temp, int p, int q) 
{ 
    int i = 0, j = 0; 
  
    for (int row = 0; row < A->row; row++) { 
        for (int col = 0; col < A->col; col++) { 
            if (row != p && col != q) { 
                setMatVal(temp, i, j++, getMatVal(A, row, col));
                if (j == A->row - 1) { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
}

void adjoint(mat* A, mat* adj) {   
    int N = 0;
    if(A->row == A->col){N = A->row;}
    if (N == 1) { 
        setMatVal(adj, 0, 0, 1); 
        return; 
    } 
  
    int sign = 1;
    mat temp;
    matInit(&temp, N-1 , N-1); 
  
    for (int i = 0; i < N; i++) { 
        for (int j = 0; j < N; j++) { 
            
            getCofactor(A, &temp, i, j); 
            sign = ((i+j)%2 == 0) ? 1 : -1; 
            setMatVal(adj, j, i, (sign)*(matDet(&temp)));
        } 
    } 
    matDestruct(&temp);
} 
