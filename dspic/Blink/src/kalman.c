#include "kalman.h"
extern  char buffer[150];
extern serial Serial1;
extern float Ts;
mat p, v, Rq, u;
mat  R, Fm, Gm, Hm, bias_p, bias_v, bias_u,Pm, Q1, Q2, ye, KalmanGain, p_gps, delta;
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
    
    mat aux1,aux2,aux3,aux4;
    matInit(&aux1, Gm.col, Gm.row);
    matInit(&aux2, Fm.col, Fm.row);
    matInit(&aux3, Gm.col, Pm.row);
    matInit(&aux4, Fm.row, Fm.col);
    
    /*printf("GM row %d, Gm col %d\n", Gm.row, Gm.col);
    for( int i = 0; i < 9; i++ ){
        for( int j = 0; j < 6; j++ ){
            printf("%lf\t",aux2.val[i][j]);
        }
        printf("\n");
    }
    
    for( int i = 0; i < 3; i++ ){
        for( int j = 0; j < 3; j++ ){
            printf("%lf\t",Q1.val[i][j]);
        }
        printf("\n");
    }

    for( int i = 0; i < 3; i++ ){
        for( int j = 0; j < 3; j++ ){
            printf("i: %d j:%d ",i,j);
            aux2.val[i][j] = Q1.val[i][j];
            printf("%lf \n",Q1.val[i][j]);
        }
        printf("\n");
    }*/
    //printf("v1:\n");
    for( int i = 3; i < 6; i++ ){
        for( int j = 3; j < 6; j++ ){
            aux1.val[i][j] = Q2.val[i-3][j-3];
        }
    }
    //serialWriteString(&Serial1, "el original aux1 1\n");
    //printMat(&Pm);
    
    
    getMatFm();
    getMatGm();
    
    
    //matTrans(&aux4,&Fm);
    //matTrans(&aux5,&Gm);
    
    matTrans(&aux2,&Fm);
    matTrans(&aux3,&Gm);

    
    /*matMult(&aux1,&Fm,&Pm);
    matMult(&aux1,&aux1,&aux4);

    //printf("v3:\n");
    matMult(&aux3,&Gm,&aux2);
    matMult(&aux4,&aux3,&aux5);
    matAdd(&Pm,&aux1,&aux4);*/
    
    //serialWriteString(&Serial1, "Op1\n");
    matMult(&Pm,&Fm,&Pm);
    serialWriteString(&Serial1, "Pm op1 \n");
    printMat(&Pm);
    //serialWriteString(&Serial1, "Op2\n");
    matMult(&Pm,&Pm,&aux2);
    //serialWriteString(&Serial1, "el Pm1\n");
    serialWriteString(&Serial1, "Pm op2 \n");
    printMat(&Pm);
    serialWriteString(&Serial1, "Pm sin cambio\n");
    printMat(&Pm);
    //serialWriteString(&Serial1, "Op3\n");
    matMult(&Gm,&Gm,&aux1);
    //serialWriteString(&Serial1, "el aux1\n");
    //printMat(&aux1);
    //serialWriteString(&Serial1, "el Gm\n");
    //printMat(&Gm);
    //serialWriteString(&Serial1, "el Pm2\n");
    serialWriteString(&Serial1, "sigo sin cambiar\n");
    printMat(&Pm);
    //serialWriteString(&Serial1, "Op4\n");
    matMult(&aux4,&Gm,&aux3);
    //serialWriteString(&Serial1, "aux3\n");
    //printMat(&aux3);
    //serialWriteString(&Serial1, "aux4\n");
    //printMat(&aux4);
    //printMat(&Pm);
    //serialWriteString(&Serial1, "Op5 raa\n");
    //printMat(&Pm);
    matAdd(&Pm,&Pm,&aux4);
    //printMat(&Pm);

    serialWriteString(&Serial1, "Fin ops\n");
    matDestruct(&aux1);
    matDestruct(&aux2);
    matDestruct(&aux3);
    matDestruct(&aux4);
    serialWriteString(&Serial1, "Fin UpdatePm\n");
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