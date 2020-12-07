#include "kalman.h"
extern  char buffer[150];
extern serial Serial1;
extern float Ts;
mat p, v, Rq,Rc, u, s;
mat Fm, Gm, Hm, bias_p, bias_v, bias_u,Pm, Q1, Q2, Q12, ye, KalmanGain, p_gps, delta;

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

void initMatGlobal(){
    matInit(&Rq, 3, 3);
    matInit(&u, 3, 1);
    matInit(&s, 3, 1);
    matInit(&p, 3, 1);
    matInit(&v, 3, 1);
    matInit(&bias_p, 3, 1);
    matInit(&bias_u, 3, 1);
    matInit(&bias_v, 3, 1);
    matInit(&ye, 3, 1);
    matInit(&p_gps,3, 1);
    matInit(&KalmanGain, 9, 3);    
    matInit(&Pm, 9, 9);
    matInit(&delta, 9, 1);

    matInit(&Rc, 3, 3);
    for(int i = 0; i < 3;  i++) setMatVal(&Rc,i,i,0.01);

    matInit(&Q12, 6, 6); 
    for(int i = 0; i < 6;  i++) setMatVal(&Q12,i,i,1);

    matInit(&Fm, 9, 9);
    for(int i = 0; i < 9 ; i++) setMatVal(&Fm, i, i, 1);
    for(int i = 0; i < 3; i++) setMatVal(&Fm, i, i+3, Ts);

    matInit(&Hm, 3, 9); 
    for(int i = 0; i <3; i++) setMatVal(&Hm, i, i,1);

    matInit(&Gm,9,6);
    for( int i = 6;  i < 9; i++ ) setMatVal(&Gm,i,i-3,1);
    
    for( int i = 3;  i < 6; i++ ){
        for( int j = 0; j < 3 ; j++) 
        setMatVal(&Gm, i, j, Ts*getMatVal(&Rq, i-3, j));
    } 
    
    matInit(&Q1, 3, 3); 
    for(int i = 0; i < 3; i++) setMatVal(&Q1, i, 0, 1);
    
    matInit(&Q2, 3, 3); 
    for(int i = 0; i < 3; i++) setMatVal(&Q2, i, 0, 1);
}

void kynematics(){
    mat aux1, aux2;
    //serialWriteString(&Serial1,"kynematics\n");
    matInit(&aux1, p.row, p.col);
    matScale(&aux1, &v, Ts);

    matInit(&aux2, p.row, p.col);
    matMult(&aux2, &Rq, &u);

    setMatVal(&aux2, 2, 0, getMatVal(&aux2, 2, 0) + 10.1);

    matScale(&aux2, &aux2, Ts);    

    matAdd(&v, &v, &aux2);
   
    matScale(&aux2, &aux2, Ts/2);

    matAdd(&p, &p, &aux1);
    matAdd(&p, &p, &aux2);
    
    //printMat(&p, "p\n");

    matDestruct(&aux1);
    matDestruct(&aux2);
}


void getMatFm(){
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            setMatVal(&Fm, i + 3 ,j + 6, Ts*getMatVal(&Rq, i, j));
}

void getMatGm(){
    for( int i = 0;  i < 3; i++ ){
        for( int j = 0; j < 3; j++) 
            setMatVal(&Gm, i+3, j, Ts*getMatVal(&Rq, i,j));
    } 
}

void UpdatePm(){
    
    //serialWriteString(&Serial1,"Update Pm\n");
    mat aux1,aux2,aux3,aux4,aux5;
    matInit(&aux1, 9, 9);
    matInit(&aux2, 6, 9);
    matInit(&aux3, 9, 9);
    matInit(&aux4, 9, 9);
    matInit(&aux5, 9, 6);
    
    getMatFm();
    getMatGm();
    
    //printMat(&Fm,"Fm\n");
    //printMat(&Gm, "Gm\n");

    //printMat(&Pm, "Pm_ant\n");
    //printMat(&Q12, "Q12\n");

    matTrans(&aux1,&Fm);
    matTrans(&aux2,&Gm);
    
    matMult(&aux4,&Fm,&Pm);
    matMult(&Pm,&aux4,&aux1);
    matMult(&aux5,&Gm,&Q12);
    matMult(&aux3,&aux5,&aux2);
    //matCopy(&Gm, &aux5);
    matAdd(&Pm,&Pm,&aux3);

    //printMat(&Pm, "Pm\n");

    matDestruct(&aux1);
    matDestruct(&aux2);
    matDestruct(&aux3);
    matDestruct(&aux4);
    matDestruct(&aux5);    
}
void getKalmanGain(){
    
    //serialWriteString(&Serial1, "Get Kalman Gain\n");
    mat aux1, aux2, aux3,aux4;


    matInit(&aux1, 3, 3);
    matInit(&aux2, 9, 3);   
    matInit(&aux3, 9, 3);
    matInit(&aux4, 3, 3);

    matTrans(&aux3, &Hm);
    matMult(&aux2, &Pm, &aux3);

    //printMat(&Hm, "Hm\n");
    //printMat(&Pm, "Pm\n");
    
    //printMat(&aux2, "aux2\n");

    matMult(&aux1, &Hm, &aux2);
    matAdd(&aux1, &aux1, &Rc);
    matInv3(&aux4, &aux1);
    //printMat(&aux1, "aux1\n");

    //printMat(&aux4, "aux4\n");

    matMult(&KalmanGain, &aux2, &aux4);
    
    //printMat(&KalmanGain, "KalmanGain\n");

    matDestruct(&aux1);
    matDestruct(&aux2);
    matDestruct(&aux3);
    matDestruct(&aux4);
}

void UpdatePmCovGPS(){
    //serialWriteString(&Serial1, "Update Pm Cov GPS\n");
    mat aux1,aux2;
    matInit(&aux1, 9, 9);
    matInit(&aux2, 9, 9);
    ////serialWriteString(&Serial1, "Matriz \n");
    ////printMat(&aux1);
    ////serialWriteString(&Serial1, "KG \n");
    ////printMat(&KalmanGain);
    ////serialWriteString(&Serial1, "Hm \n");
    ////printMat(&Hm);
    matMult(&aux1, &KalmanGain, &Hm);
    ////serialWriteString(&Serial1, "aux1 \n");
    ////printMat(&aux1);
    
    //printMat(&aux1, "aux1\n");

    for(int i = 0; i < 9; i++) 
        setMatVal(&aux1, i , i, 1 - getMatVal(&aux1, i , i));
    
    //printMat(&aux1, "aux1\n");
    
    matMult(&aux2, &aux1, &Pm);
    matCopy(&Pm, &aux2);
    
    //printMat(&Pm, "Pm\n");
    matDestruct(&aux1);
    matDestruct(&aux2);
}

void getBias(){

    //serialWriteString(&Serial1, "Get Bias\n");
    //sprintf(buffer, "hola 3\n");
    ////serialWriteString(&Serial1, buffer);

    matSubs(&ye, &p_gps,  &p);
    //printMat(&ye, "ye\n");

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

    //printMat(&delta, "delat\n");
}
int cont = 0;
void kalmanUpdate(){
    
    matAdd(&u, &s, &bias_u);
    //printMat(&u," u\n");
    kynematics();
    
    UpdatePm();
    cont++;
    //sprintf(buffer, "Contador: %d\n", cont);
    //serialWriteString(&Serial1, buffer);
    if (cont>100){
        //sprintf(buffer, "Contador: %d\n", cont);
        //serialWriteString(&Serial1, buffer);
        
        setMatVal(&p_gps, 0, 0, 0);
        setMatVal(&p_gps, 1, 0, 0);
        
        getKalmanGain();

        ////serialWriteString(&Serial1, "hola \n");

        getBias();
        ////serialWriteString(&Serial1, "hola1\n");
        
        UpdatePmCovGPS();
        //serialWriteString(&Serial1, "hola2\n");
        
        matAdd(&p, &p, &bias_p);
        matAdd(&v, &v, &bias_v);
        cont = 0;
    }

}