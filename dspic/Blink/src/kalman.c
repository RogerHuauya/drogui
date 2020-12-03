#include "kalman.h"
void kynematics(mat* p, mat* vel, mat* u, mat* Rq, float Ts){
    mat aux1, aux2;
    matInit(&aux1, p->row, p->col);
    matScale(&aux1, vel, Ts);

    matInit(&aux2, p->row, p->col);
    matMult(&aux2, Rq, u);
    setMatVal(&aux2, 2, 0, getMatVal(&aux2, 0, 2)-0.5);
    matScale(&aux2, &aux2, Ts);
    
    matAdd(vel, vel, &aux2);

    matScale(&aux2, &aux2, Ts/2);

    matAdd(p, p, &aux1);
    matAdd(p, p, &aux2);
}