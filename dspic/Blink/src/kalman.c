#include "kalman.h"
extern  char buffer[150];
extern serial Serial1;
extern float Ts;
mat p, v, Rq, u;
mat  R, Fm, Gm, Hm, bias_p, bias_v, bias_u,Pm, Q1, Q2, Q12, ye, KalmanGain, p_gps, delta;
void initMatGlobal(){
    matInit(&Rq, 3, 3);
    matInit(&R, 3, 3);
    matInit(&u, 3, 1);
    matInit(&p, 3, 1);
    matInit(&v, 3, 1);
    matInit(&Fm, 9, 9);
    matInit(&bias_u, 3, 1);
    matInit(&ye, 3, 1);
    matInit(&KalmanGain, 12, 3);
    matInit(&Q12, 6, 6); for(int i = 0; i < 6;  i++) setMatVal(&Q12,i,i,1);
    for(int i = 0; i < 9 ; i++) setMatVal(&Fm, i, i, 1);
    for(int i = 0; i < 3; i++) setMatVal(&Fm, i, i+3, Ts);

    matInit(&Hm, 3, 9); for(int i = 0; i <3; i++) setMatVal(&Hm, i, i,1);

    matInit(&Gm,9,6);
    
    for( int i = 6;  i < 9; i++ ) setMatVal(&Gm,i,i-3,1);
    
    for( int i = 3;  i < 6; i++ ){
        for( int j = 0; j < 3 ; j++) setMatVal(&Gm, i, j, Ts*getMatVal(&Rq, i-3, j));
    } 
    
    matInit(&Pm, 9, 9);
    matInit(&Q1, 3, 3); for(int i = 0; i < 3; i++) setMatVal(&Q1, i, 0, 1);
    matInit(&Q2, 3, 3); for(int i = 0; i < 3; i++) setMatVal(&Q2, i, 0, 1);
}
void kynematics(){
    mat aux1, aux2;
    
    matInit(&aux1, p.row, p.col);
    matScale(&aux1, &v, Ts);

    matInit(&aux2, p.row, p.col);
    matMult(&aux2, &Rq, &u);

    setMatVal(&aux2, 2, 0, getMatVal(&aux2, 2, 0)+10.1);


    matScale(&aux2, &aux2, Ts);    

    matAdd(&v, &v, &aux2);
   

    matScale(&aux2, &aux2, Ts/2);

    matAdd(&p, &p, &aux1);
    matAdd(&p, &p, &aux2);
    
   
    matDestruct(&aux1);
    matDestruct(&aux2);
}


void getMatFm(){
    for(int i = 0; i < 3; i++)
    for(int j = 0; j < 3; j++)
        setMatVal(&Fm, i+3,j + 6, Ts*getMatVal(&Rq, i, j));
}

void getMatGm(){
    for( int i = 3;  i < 6; i++ ){
        for( int j = 0; j < 3; j++) setMatVal(&Gm, i, j, Ts*getMatVal(&Rq, i-3,j));
    } 
}
void printMat(mat* R){
    for( int i = 0; i < (R->row); i++ ){
        for( int j = 0; j < (R->col); j++ ){
            sprintf(buffer,"%lf\t",R->val[i][j]);
            serialWriteString(&Serial1, buffer);
        } 
        serialWriteString(&Serial1, "\n");
    }
}

void UpdatePm(){
    
    mat aux1,aux2,aux3;
    matInit(&aux1, 9, 9);
    matInit(&aux2, 6, 9);
    matInit(&aux3, 9, 9);
    
    getMatFm();
    getMatGm();
    
    matTrans(&aux1,&Fm);
    matTrans(&aux2,&Gm);
    
    matMult(&Pm,&Fm,&Pm);
    matMult(&Pm,&Pm,&aux1);
    matMult(&Gm,&Gm,&Q12);
    matMult(&aux4,&Gm,&aux2);
    matAdd(&Pm,&Pm,&aux3);
    
    matDestruct(&aux1);
    matDestruct(&aux2);
    matDestruct(&aux3);
}
void getKalmanGain(){
    mat aux1, aux2, aux3;

    matInit(&aux1, 3, 3);
    matInit(&aux2, 9, 3);   
    matInit(&aux3, 9, 3);

    matTrans(&aux3, &Hm);
    matMult(&aux2, &Pm, &aux3);

    matMult(&aux1, &Hm, &aux2);
    matAdd(&aux1, &aux1, &R);
    matInv3(&aux3, &aux1);
    matMult(&KalmanGain, &aux2, &aux3);
}

void UpdatePmCovGPS(){
    mat aux1;
    matInit(&aux1, 9, 9);
    matMult(&aux1, &KalmanGain, &Hm);
    for(int i = 0; i < 9; i++) setMatVal(&aux1, i , i, 1 - getMatVal(&aux1, i , i));
    matMult(&Pm, &aux1, &Pm);
    matDestruct(&aux1);
}

void getBias(){
    matSubs(&ye, &p_gps,  &p);
    matMult(&delta, &KalmanGain, &ye);

    setMatVal(&delta, 6, 0, getMatVal(&delta, 6, 0) + getMatVal(&bias_u, 0, 0));
    setMatVal(&delta, 7, 0, getMatVal(&delta, 7, 0) + getMatVal(&bias_u, 1, 0));
    setMatVal(&delta, 8, 0, getMatVal(&delta, 8, 0) + getMatVal(&bias_u, 2, 0));

    setMatVal(&bias_p, 0, 0, getMatVal(&delta, 0, 0));
    setMatVal(&bias_p, 1, 0, getMatVal(&delta, 1, 0));
    setMatVal(&bias_p, 2, 0, getMatVal(&delta, 2, 0));

    setMatVal(&bias_v, 0, 0, getMatVal(&delta, 3, 0));
    setMatVal(&bias_v, 1, 0, getMatVal(&delta, 4, 0));
    setMatVal(&bias_v, 2, 0, getMatVal(&delta, 5, 0));

    setMatVal(&bias_u, 0, 0, getMatVal(&delta, 6, 0));
    setMatVal(&bias_u, 1, 0, getMatVal(&delta, 7, 0));
    setMatVal(&bias_u, 2, 0, getMatVal(&delta, 8, 0));

}
int cont = 0;
void kalmanUpdate(){
    matAdd(&u, &u, &bias_u);
    //printf("1\n");
    kynematics();
    //printf("2\n");

    UpdatePm();
    //printf("3\n");
    cont++;
    if (cont>100){

        //setMatVal(&p_gps, 0, 0, getReg(GPS_X));
        //setMatVal(&p_gps, 1, 0, getReg(GPS_Y));

        //printf("4\n");
        setMatVal(&p_gps, 0, 0, 0);
        setMatVal(&p_gps, 1, 0, 0);

        getKalmanGain();
        //printf("5\n");

        getBias();
        //printf("6\n");
        
        UpdatePmCovGPS();
        //printf("7\n");

        matAdd(&p, &p, &bias_p);
        matAdd(&v, &v, &bias_v);
    }

}