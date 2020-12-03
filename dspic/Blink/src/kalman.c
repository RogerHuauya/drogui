#include "kalman.h"
void kynematics(mat* p, mat* v, mat* u, mat* Rq,float Ts){
    mat aux;
    matInit(&aux, p->row, p->col);
    matMult(&aux, &Rq, &u);
    setMatVal(&aux, 2, 0, getMatVal(&aux)-0.5);
    
}