#include "..\headers\kalman.h"

extern char buffer[150];

#define GRAVITY 1
float Ts, N;
mat p, v, Rq,Rc, u, s;
mat Fm, Gm, Hm, bias_p, bias_v, bias_u,Pm, Q12, ye, KalmanGain, p_gps, delta;

void setKalmanTsImu(float ts){
    Ts = ts;
}

void setKalmanTsGps(float ts){
    N = Ts/ts;
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
    matInit(&delta, 9, 1);

    matInit(&Rc, 3, 3);
    for(int i = 0; i < 3;  i++) setMatVal(&Rc,i,i,0.1);

    matInit(&Q12, 6, 6); 
    for(int i = 0; i < 6;  i++) setMatVal(&Q12,i,i,0.01);

    matInit(&Pm, 9, 9); 
    for(int i = 0; i < 9;  i++) setMatVal(&Pm,i,i,0.1);

    matInit(&Fm, 9, 9);
    for(int i = 0; i < 9 ; i++) setMatVal(&Fm, i, i, 1);
    for(int i = 0; i < 3; i++) setMatVal(&Fm, i, i+3, Ts);

    matInit(&Hm, 3, 9); 
    for(int i = 0; i <3; i++) setMatVal(&Hm, i, i,1);

    matInit(&Gm,9,6);
    for( int i = 6;  i < 9; i++ ) setMatVal(&Gm, i, i-3,1);
    
}

void kynematics(){
    mat aux1, aux2;

    matInit(&aux1, p.row, p.col);
    matScale(&aux1, &v, Ts);

    matInit(&aux2, p.row, p.col);
    matMult(&aux2, &Rq, &u);

    //setMatVal(&aux2, 2, 0, getMatVal(&aux2, 2, 0) + 10.0062);

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
            setMatVal(&Fm, i + 3 ,j + 6, Ts*getMatVal(&Rq, i, j));
}

void getMatGm(){
    for( int i = 0;  i < 3; i++ ){
        for( int j = 0; j < 3; j++) 
            setMatVal(&Gm, i+3, j, Ts*getMatVal(&Rq, i,j));
    } 
}

void UpdatePm(){
    
    mat aux1,aux2,aux3,aux4,aux5;
    matInit(&aux1, 9, 9);
    matInit(&aux2, 6, 9);
    matInit(&aux3, 9, 9);
    matInit(&aux4, 9, 9);
    matInit(&aux5, 9, 6);
    
    getMatFm();
    getMatGm();

    matTrans(&aux1,&Fm);
    matTrans(&aux2,&Gm);
    
    matMult(&aux4,&Fm,&Pm);
    matMult(&Pm,&aux4,&aux1);

    matMult(&aux5,&Gm,&Q12);
    matMult(&aux3,&aux5,&aux2);
    matAdd(&Pm, &Pm, &aux3);

    matDestruct(&aux1);
    matDestruct(&aux2);
    matDestruct(&aux3);
    matDestruct(&aux4);
    matDestruct(&aux5);
}
void getKalmanGain(){
    
    mat aux1, aux2, aux3,aux4;


    matInit(&aux1, 3, 3);
    matInit(&aux2, 9, 3);   
    matInit(&aux3, 9, 3);
    matInit(&aux4, 3, 3);

    matTrans(&aux3, &Hm);
    matMult(&aux2, &Pm, &aux3);

    matMult(&aux1, &Hm, &aux2);
    matAdd(&aux1, &aux1, &Rc);
    matInv3(&aux4, &aux1);

    matMult(&KalmanGain, &aux2, &aux4);

    matDestruct(&aux1);
    matDestruct(&aux2);
    matDestruct(&aux3);
    matDestruct(&aux4);
}

void UpdatePmCovGPS(){
    mat aux1,aux2;
    matInit(&aux1, 9, 9);
    matInit(&aux2, 9, 9);
    matMult(&aux1, &KalmanGain, &Hm);
    matScale(&aux1, &aux1, -1);
    for(int i = 0; i < 9; i++) 
        setMatVal(&aux1, i , i, 1 + getMatVal(&aux1, i , i));
    
    
    matMult(&aux2, &aux1, &Pm);
    matCopy(&Pm, &aux2);
    
    matDestruct(&aux1);
    matDestruct(&aux2);
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
}

void kalmanUpdateIMU(float ax, float ay, float az,float roll, float pitch, float yaw){
    
    //quaternionToR(&Rq, qw, qx, qy, qz);
    rpyToR(&Rq, roll, pitch, yaw);
    setMatVal(&u, 0, 0, ax*GRAVITY);
    setMatVal(&u, 1, 0, ay*GRAVITY);
    setMatVal(&u, 2, 0, az*GRAVITY);
    //matAdd(&u, &s, &bias_u);
    kynematics();
    UpdatePm();

    matAdd(&p, &p, &bias_p);
}

void kalmanUpdateGPS(float x_gps, float y_gps, float z_gps){

    setMatVal(&p_gps, 0, 0, x_gps);
    setMatVal(&p_gps, 1, 0, y_gps);
    setMatVal(&p_gps, 2, 0, z_gps);
    
    getKalmanGain();

    getBias();
    
    UpdatePmCovGPS();
    
    matScale(&bias_v, &bias_v, 1/1.3);
    matAdd(&v, &v, &bias_v);
    matScale(&bias_p, &bias_p, N);
}



void getPosition(float *x, float *y, float *z){
    *x = getMatVal(&p, 0, 0);
    *y = getMatVal(&p, 1, 0);
    *z = getMatVal(&p, 2, 0);
}

void clearKalman(){
        
    for(int i = 0 ; i < 3 ; i++){
        setMatVal(&p, i, 0, 0);
        setMatVal(&v, i, 0, 0);
        setMatVal(&bias_p, i, 0, 0);
    }
    for(int i = 0; i < 9;  i++){
        for(int j = 0; j < 9; j++) setMatVal(&Pm, i, j, 0);
        for(int j = 0; j < 3; j++) setMatVal(&KalmanGain, i, j, 0);
        
        setMatVal(&delta, i, 0, 0);
        setMatVal(&Pm,i,i,0.1);
    }

}