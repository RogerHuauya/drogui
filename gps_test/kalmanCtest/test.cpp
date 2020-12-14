#include <bits/stdc++.h>
using namespace std;
/********************************************
 * 
 * MATLIB_H
 * 
 *********************************************/
struct mat{
    int row, col;
    float** val;
    float* aux;
};

void matInit(mat* m, int row, int col);
void matMult(mat* ans, mat* a, mat* b);
void quaternionToR(mat* R, float q1, float q2, float q3, float q4);
double q2double(int q);
int double2q(double d);
void setMatVal(mat* m, int i, int j, float value);
float getMatVal(mat* m, int i, int j);
void matDestruct(mat* m);
void matScale(mat* ans, mat* a, float alpha);
void matAdd(mat* ans, mat* a, mat* b);
void matSubs(mat* ans, mat* a, mat* b);
void matInv3(mat* Rinv, mat* R);
void matTrans(mat* Rt,mat* R);
void matCopy(mat* des, mat* src);
void rpyToR(mat* R, float roll, float pitch, float yaw);

/******************************************
 * 
 * MATLIB_C
 * 
 *****************************************/

void matInit(mat* m, int row, int col){
    m->row = row;
    m->col = col;
    
    m->val = (float**) calloc(row, sizeof(float*));
    
	m-> aux = (float*) calloc(row*col, sizeof(float));
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
void printMat(mat* R, string s);

/******************************************************
 * 
 * KALMAN_H
 * 
 * ***************************************************/


void initMatGlobal();
void kynematics();
void getMatFm();
void getMatGm();
void UpdatePm();
void getKalmanGain();
void UpdatePmCovGPS();
void getBias();
void kalmanUpdateIMU(float ax, float ay, float az,float qw, float qx,float qy);
void kalmanUpdateGPS(float x_gps, float y_gps, float z_gps);
void setKalmanTsImu(float ts);
void getPosition(float *x, float *y, float *z);
void setKalmanTsGps(float ts);
void clearKalman();

/***********************************************************
 * 
 * KALMAN_C
 * 
***********************************************************/

#define GRAVITY 9.81
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

    setMatVal(&aux2, 2, 0, getMatVal(&aux2, 2, 0) + 10.0062);

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

void kalmanUpdateIMU(float ax, float ay, float az,float r, float pitch,float y){
    
    rpyToR(&Rq, r, pitch, y);
    setMatVal(&u, 0, 0, ax*GRAVITY);
    setMatVal(&u, 1, 0, ay*GRAVITY);
    setMatVal(&u, 2, 0, az*GRAVITY);
    
    //matAdd(&u, &u, &bias_u);
    
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
    //matAdd(&p, &p, &bias_p);
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

vector<double> t_imu, roll, pitch, yaw, ax, ay, az;
double t, rol, pit, ya, axx, ayy, azz;

vector<double> t_gps, x, y;
double xx, yy;

void printMat(mat* R, string s){    
    cout << s<< endl;
    for( int i = 0; i < (R->row); i++ ){
        for( int j = 0; j < (R->col); j++ ){
            printf("%lf\t",R->val[i][j]);
        } 
        printf( "\n");
    }
    printf("\n");
}


int main(){
    while(cin>>t && t != -1){
        t_imu.push_back(t);
        cin >> rol >> pit >> ya >> axx >> axx >> axx >> axx >> ayy >> azz;
        roll.push_back(rol);
        pitch.push_back(pit);
        yaw.push_back(ya);
        ax.push_back(axx);
        ay.push_back(ayy);
        az.push_back(azz);
    }
    
    

    while(cin>>t){
        t_gps.push_back(t);
        cin >> xx >> xx >> xx >> yy;
        x.push_back(xx);
        y.push_back(yy);
    }
    
    setKalmanTsImu(0.02);
    setKalmanTsGps(1);
    initMatGlobal();
    
    /*rpyToR(&Rq, 2, 0, 1);
    UpdatePm();
    printMat(&Pm, "Pm");
    printMat(&Fm, "Fm");
    printMat(&Gm, "Gm");

    getKalmanGain();

    printMat(&KalmanGain, "Kalman Gain");

    setMatVal(&p_gps, 0,0, 1);
    setMatVal(&p_gps, 1,0, 2);
    setMatVal(&p_gps, 2,0, 3);
    
    setMatVal(&bias_u, 0,0, 1);
    setMatVal(&bias_u, 1,0, 2);
    setMatVal(&bias_u, 2,0, 3);

    getBias();

    printMat(&delta, "delta");

    UpdatePmCovGPS();
    
    printMat(&Pm, "Pm");

    cout << getMatVal(&Pm, 3, 0) << "\t" << getMatVal(&Pm, 8, 5) << endl;
    */

    
    
    int j = 0;
    for(int i = 0; i < t_imu.size(); i++){
        if(t_imu[i] < 20) continue;

        kalmanUpdateIMU(ax[i], ay[i], az[i], roll[i], pitch[i], yaw[i]);
        if(t_gps[j] == t_imu[i]){
            
            //cout << x[j] << "\t" << y[j] << endl;
            j++;
            kalmanUpdateGPS(x[j],y[j], 0);  
            if(j == t_gps.size()) break;  
        }
        cout << getMatVal(&p, 0, 0) << "\t" << getMatVal(&p, 1, 0) << endl;
        
    }
}