#include "kalman.h"
#include "serial.h"
extern  char buffer[100];
extern serial Serial1;
void kynematics(mat* p, mat* vel, mat* u, mat* Rq, float Ts){
    mat aux1, aux2;
    
    matInit(&aux1, p->row, p->col);
    matScale(&aux1, vel, Ts);

    matInit(&aux2, p->row, p->col);
    matMult(&aux2, Rq, u);
    /*
    for(int i = 0 ;i <3 ; i++){
        for(int j = 0 ;j <1 ; j++){
            sprintf(buffer, "%.6lf\t", getMatVal(&aux2,i,j)), serialWriteString(&Serial1, buffer);
        }
        serialWriteString(&Serial1, "\n");
    }
    
    serialWriteString(&Serial1, "\n");*/



    setMatVal(&aux2, 2, 0, getMatVal(&aux2, 2, 0)+10.1);


    matScale(&aux2, &aux2, Ts);    
    /*
    for(int i = 0 ;i <3 ; i++){
        for(int j = 0 ;j <1 ; j++){
            sprintf(buffer, "%.6lf\t", getMatVal(&aux2,i,j)), serialWriteString(&Serial1, buffer);
        }
        serialWriteString(&Serial1, "\n");
    }
    
    serialWriteString(&Serial1, "\n");*/

    matAdd(vel, vel, &aux2);
   

    matScale(&aux2, &aux2, Ts/2);

    matAdd(p, p, &aux1);
    matAdd(p, p, &aux2);
    
   
    matDestruct(&aux1);
    matDestruct(&aux2);
}