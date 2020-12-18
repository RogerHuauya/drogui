#include "matlib.h"
#include "serial.h"
extern char buffer[50];
extern serial Serial1;

void matInit(mat* m, int row, int col){
    m->row = row;
    m->col = col;
    
    m->val = (float**) calloc(row, sizeof(float*));

	m-> aux = calloc(row*col, sizeof(float));
    for (int i = 0; i < row; i++) {
            m->val[i] = m-> aux + i*col;
    }
}

void matMult(mat* ans, mat* a, mat* b){
    for(int i = 0 ;i < a->row; i++)
        for(int j = 0 ; j< b->col ; j++){
            float aux = 0;
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
    float det = ((R->val[0][0])*(R->val[1][1])*(R->val[2][2])) +((R->val[0][1])*(R->val[1][2])*(R->val[2][0]))
     + ((R->val[0][2])*(R->val[1][0])*(R->val[2][1])) - ((R->val[0][2])*(R->val[1][1])*(R->val[2][0])) 
     - ((R->val[0][1])*(R->val[1][0])*(R->val[2][2])) - ((R->val[0][0])*(R->val[1][2])*(R->val[2][1]));
    
    return det;
}
void matInv3(mat* Rinv, mat* R){
    float val_det = 0;
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
    
    R->val[0][0] = 2*(n*n + ex*ex) - 1;
    R->val[0][1] = 2*(ex*ey - n*ez);
    R->val[0][2] = 2*(ex*ez + n*ey);

    R->val[1][0] = 2*(ex*ey + n*ez);
    R->val[1][1] = 2*(n*n + ey*ey) - 1;
    R->val[1][2] = 2*(ey*ez - n*ex);
    
    R->val[2][0] = 2*(ex*ez - n*ey);
    R->val[2][1] = 2*(ey*ez + n*ex);
    R->val[2][2] = 2*(n*n + ez*ez) - 1;
}

void rpyToR(mat* R, float roll, float pitch, float yaw){
    R->row = R->col = 3;
    
    R->val[0][0] = cos(yaw)*cos(pitch);
    R->val[0][1] = cos(yaw)*sin(pitch)*sin(roll) - sin(yaw)*cos(roll);
    R->val[0][2] = cos(yaw)*sin(pitch)*cos(roll) + sin(yaw)*sin(roll);

    R->val[1][0] = sin(yaw)*cos(pitch);
    R->val[1][1] = sin(yaw)*sin(pitch)*sin(roll) + cos(yaw)*cos(roll);
    R->val[1][2] = sin(yaw)*sin(pitch)*cos(roll) - cos(yaw)*sin(roll);
    
    R->val[2][0] = -sin(pitch);
    R->val[2][1] = cos(pitch)*sin(roll);
    R->val[2][2] = cos(pitch)*cos(roll);
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


void printMat(mat* R, char* s){    
    serialWriteString(&Serial1, s);
    for( int i = 0; i < (R->row); i++ ){
        for( int j = 0; j < (R->col); j++ ){
            sprintf(buffer,"%lf\t",R->val[i][j]);
            serialWriteString(&Serial1, buffer);
        } 
        serialWriteString(&Serial1, "\n");
    }
    serialWriteString(&Serial1, "\n");
}

