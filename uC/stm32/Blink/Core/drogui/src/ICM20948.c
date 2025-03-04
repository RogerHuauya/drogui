#include "ICM20948.h"
#include "i2c.h"
#include "matlib.h"
#include "registerMap.h"
#include "sensorsTasks.h"
#include "utils.h"
#include "serial.h"

#define G 9.81

//char buffcal[500]="";

int updateIcmCalibOffset(icm20948* m){
    int ans = 0;

    #if IMU == ICM20948

        if( m->off_gx != getReg(GYR_X_OFF))  ans |= 2, m->off_gx = getReg(GYR_X_OFF);
        if( m->off_gy != getReg(GYR_Y_OFF))  ans |= 2, m->off_gy = getReg(GYR_Y_OFF);
        if( m->off_gz != getReg(GYR_Z_OFF))  ans |= 2, m->off_gz = getReg(GYR_Z_OFF);

        if( m->off_ax  != getReg(ACC_X_OFF)) ans |= 1, m->off_ax = getReg(ACC_X_OFF);
        if( m->off_ay  != getReg(ACC_Y_OFF)) ans |= 1, m->off_ay = getReg(ACC_Y_OFF); 
        if( m->off_az  != getReg(ACC_Z_OFF)) ans |= 1, m->off_az = getReg(ACC_Z_OFF);
        if( m->scl_acc != getReg(ACC_SCALE)) ans |= 1, m->scl_acc =  getReg(ACC_SCALE);

    #endif

/*    if( m->off_mx != getReg(MAG_X_OFF)) ans |= 4, m->off_mx =   getReg(MAG_X_OFF);
    if( m->off_my != getReg(MAG_Y_OFF)) ans |= 4, m->off_my = getReg(MAG_Y_OFF);     
    if( m->off_mz != getReg(MAG_Z_OFF)) ans |= 4, m->off_mz = getReg(MAG_Z_OFF);
    
    if( m->scl_magx != getReg(MAG_X_SCALE)) ans |= 4, m->scl_magx = getReg(MAG_X_SCALE);
    if( m->scl_magy != getReg(MAG_Y_SCALE)) ans |= 4, m->scl_magy = getReg(MAG_Y_SCALE);     
    if( m->scl_magz != getReg(MAG_Z_SCALE)) ans |= 4, m->scl_magz = getReg(MAG_Z_SCALE);*/
    
    if(ans != 0) ans |= 8;

    return ans;
}

void initIcm(icm20948* m){
    I2CwriteByte(ICM20948_ADDRESS, CHANGE_BANK, 0);
    I2CwriteByte(ICM20948_ADDRESS, PWR_MGT, 9);
    I2CwriteByte(ICM20948_ADDRESS, CHANGE_BANK, 2<<4);
    I2CwriteByte(ICM20948_ADDRESS, ICM_GYRO_SMPLRT_DIV, 0);
    I2CwriteByte(ICM20948_ADDRESS, ICM_GYRO_CONFIG1, ICM_GYRO_FULL_SCALE_2000_DPS);
    I2CwriteByte(ICM20948_ADDRESS, ICM_ACCEL_CONFIG1,ICM_ACC_FULL_SCALE_2_G);
    I2CwriteByte(ICM20948_ADDRESS, ICM_ACCEL_SMPLRT_DIV_MSB, 0);
    I2CwriteByte(ICM20948_ADDRESS, ICM_ACCEL_SMPLRT_DIV_LSB, 0);
    I2CwriteByte(ICM20948_ADDRESS, CHANGE_BANK, 0);
    HAL_Delay(1000);

    // Init magnetometer
    I2CwriteByte(ICM20948_ADDRESS, INT_PIN_CFG, 2);
    I2CwriteByte(ICM20948_ADDRESS, USER_CTRL, 0);
    
    m->scl_acc = 1;

    initIcmFiltGyro(&(m->fGyroX)), initIcmFiltGyro(&(m->fGyroY)), initIcmFiltGyro(&(m->fGyroZ));
    initIcmFiltAcc(&(m->fAccX)), initIcmFiltAcc(&(m->fAccY)), initIcmFiltAcc(&(m->fAccZ));
}

void initIcmFiltGyro(icmFiltGyro *fg){

    initFilter(&(fg->first), 6 , k_1_10, v_1_10);
    initFilter(&(fg->second), 5 , k_3_10, v_3_10);
    initDNotchFilter(&(fg->third), 64, 30, 1000, 3, 5);
    initDNotchFilter(&(fg->fourth), 64, 30, 1000, 5, 5);
}

void initIcmFiltAcc(icmFiltAcc *fa){
    
    initFilter(&(fa->first), 4 , k_1_20, v_1_20);
    initDNotchFilter(&(fa->second), 64, 40, 1000, 1, 1);
}


float computeIcmFiltGyro(icmFiltGyro *fg, float val){
    val = computeFilter(&(fg->first), val);
    val = computeFilter(&(fg->second), val);
    val = computeDNotch(&(fg->third), val);
    val = computeDNotch(&(fg->fourth), val);
    //val = computeFilter(&(fg->fifth), val);
    return val / 16.4;
}

float computeIcmFiltAcc(icmFiltAcc *fa, float val){
    val = computeFilter(&(fa->first), val);

    val = computeDNotch(&(fa->second), val);
    return val;
}



void cleanIcmFiltGyro(icmFiltGyro *fg){
    cleanFilter(&(fg->first));
    cleanFilter(&(fg->second));
    cleanDNotch(&(fg->third));
    cleanDNotch(&(fg->fourth));
    cleanFilter(&(fg->fifth));
}

void cleanIcmFiltAcc(icmFiltAcc *fa){
    cleanFilter(&(fa->first));
    cleanDNotch(&(fa->second));
}


void readIcmRawAcc(icm20948* m){ // m/s^2
    uint8_t Buf[6];
    I2Cread(ICM20948_ADDRESS, ICM_ACCEL_XOUT_H, 6, Buf);
    int16_t _ax =   (Buf[0]<<8) | Buf[1];
    int16_t _ay =   (Buf[2]<<8) | Buf[3];
    int16_t _az =   (Buf[4]<<8) | Buf[5];
    
    m -> raw_ax = _ax;
    m -> raw_ay = _ay;
    m -> raw_az = _az; 
}

void readIcmRawGyro(icm20948* m){ // degrees/sec
    
    uint8_t Buf[6];
    I2Cread(ICM20948_ADDRESS, ICM_GYRO_XOUT_H, 6, Buf);
    int16_t _gx =   (Buf[0] << 8) | Buf[1];
    int16_t _gy =   (Buf[2] << 8) | Buf[3];
    int16_t _gz =   (Buf[4] << 8) | Buf[5];
    m -> raw_gx = _gx;
    m -> raw_gy = _gy;
    m -> raw_gz = _gz; 
}

void readIcmRawMag(icm20948* m){ // m/s^2

    uint8_t Buf[7];
    I2Cread(ICM_MAG_ADDRESS, 0x11, 6, Buf);
    int16_t _mx = -((Buf[1]<<8) | Buf[0]);
    int16_t _my = -((Buf[3]<<8) | Buf[2]);
    int16_t _mz = -((Buf[5]<<8) | Buf[4]);
    m -> raw_mx = _mx;
    m -> raw_my = _my;
    m -> raw_mz = _mz; 
}

void readIcmFiltAcc(icm20948* m){ // m/s^2
    
    readIcmRawAcc(m);
    m -> filt_ax = computeIcmFiltAcc(&(m->fAccX), m->raw_ax);
    m -> filt_ay = computeIcmFiltAcc(&(m->fAccY), m->raw_ay);
    m -> filt_az = computeIcmFiltAcc(&(m->fAccZ), m->raw_az); 
}

void readIcmFiltGyro(icm20948* m){ // degrees/sec
    
    readIcmRawGyro(m);
    m -> filt_gx = computeIcmFiltGyro(&(m->fGyroX), m->raw_gx);
    m -> filt_gy = computeIcmFiltGyro(&(m->fGyroY), m->raw_gy);
    m -> filt_gz = computeIcmFiltGyro(&(m->fGyroZ), m->raw_gz);
}


void readIcmAcc(icm20948* m){ // m/s^2
    readIcmFiltAcc(m);
    m -> ax = (m->filt_ax + m->off_ax)/m->scl_acc;
    m -> ay = (m->filt_ay + m->off_ay)/m->scl_acc;
    m -> az = (m->filt_az + m->off_az)/m->scl_acc; 
}


void readIcmGyro(icm20948* m){ // degrees/sec
    readIcmFiltGyro(m);
    m -> gx = m->filt_gx + m->off_gx;
    m -> gy = m->filt_gy + m->off_gy;
    m -> gz = m->filt_gz + m->off_gz; 
}
/*
void readIcmMag(icm20948* m){ // m/s^2
    uint8_t Buf[7];
    I2Cread(MAG_ADDRESS, 0x03, 7, Buf);
    
    _mx = -((Buf[3]<<8) | Buf[2]);
    _my = -((Buf[1]<<8) | Buf[0]);
    _mz = -((Buf[5]<<8) | Buf[4]);
    //-226.43	-112.81	131.13	119.36	127.29  129.95


    m -> mx = (_mx - m->off_mx)/m->scl_magx;
    m -> my = (_my - m->off_my)/m->scl_magy;
    m -> mz = (_mz - m->off_mz)/m->scl_magz; 
}
*/
// 120 260 380

bool icmQuiet(icm20948* m, int n, float treshold, bool cal){
    
    float max_gyro[3] = {0,0,0};
    float min_gyro[3] = {0,0,0};
    float acum_gyro[3] = {0,0,0};

    //HAL_UART_Transmit(&huart2, (uint8_t*) "Quiet\n", 8, 100);

    for(int i = 0; i < n; i++){
        
        readIcmFiltGyro(m);   

        if(i == 0){
            max_gyro[0] = m->filt_gx, max_gyro[1] = m->filt_gy, max_gyro[2] = m->filt_gz;
            min_gyro[0] = m->filt_gx, min_gyro[1] = m->filt_gy, min_gyro[2] = m->filt_gz;
        }
        else{
                max_gyro[0] = fmax(max_gyro[0], m->filt_gx);
                max_gyro[1] = fmax(max_gyro[1], m->filt_gy);
                max_gyro[2] = fmax(max_gyro[2], m->filt_gz);
                
                min_gyro[0] = fmin(min_gyro[0], m->filt_gx);
                min_gyro[1] = fmin(min_gyro[1], m->filt_gy);
                min_gyro[2] = fmin(min_gyro[2], m->filt_gz);
        }

        acum_gyro[0] += m ->filt_gx;
        acum_gyro[1] += m ->filt_gy;
        acum_gyro[2] += m ->filt_gz;


        HAL_Delay(1);
    }
    
    //serialPrintf("%f\t%f\t%f\n", max_gyro[0]-min_gyro[0], max_gyro[1]-min_gyro[1], max_gyro[2]-min_gyro[2] );

    if( ( max_gyro[0]-min_gyro[0] < (treshold + 2.0 /*1.8*/) ) &&\
        ( max_gyro[1]-min_gyro[1] < (treshold + 4.0 /*3.8*/) ) && \
        ( max_gyro[2]-min_gyro[2] < (treshold + 5.0) ) ){
        if(cal){
            m->off_gx = -1.0*acum_gyro[0]/n;
            m->off_gy = -1.0*acum_gyro[1]/n;
            m->off_gz = -1.0*acum_gyro[2]/n;
        }
        return true;
    }
    return false;
}


void calibrateIcmGyro(icm20948* m){
    //HAL_UART_Transmit(&huart2, (uint8_t*) "CalibG\n", 8, 100);
    setReg(CAL_GYR,0);
    while(!icmQuiet(m,1000,0, true));
    
    setReg(GYR_X_OFF, m -> off_gx);
    setReg(GYR_Y_OFF, m -> off_gy);
    setReg(GYR_Z_OFF, m -> off_gz);

    setReg(CAL_GYR, 100);
    #if PORT == LED 
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    #endif
}




void calibrateIcmAccel(icm20948* m){
    
    setReg(CAL_ACC,0);

    int neq = 4, nval = 2;
    int tot = neq + nval;

    float acc[tot][3], scale, aux;
    for(int i = 0; i < tot; i++){
        for(int j = 0; j < 3; j++){
            acc[i][j] = 0;
        }    
    }

    mat A, b, ans;
    matInit(&A,3,3);
    matInit(&b,3,1);
    matInit(&ans,3,1);

    int head = 0, cnt = 0;
    bool done = false, valid;
    
    while(!done){
        while(!icmQuiet(m, 100, 2, false));
        readIcmRawAcc(m);
        valid = true;
        for(int i = 1 ; i <= cnt ; i++){
            int j = (head - i + tot) % tot;
            // Serial.println("icmQuiet ");
            float d = dis3d(m->raw_ax, m->raw_ay, m->raw_az, acc[j][0],acc[j][1],acc[j][2]);
            
            //sprintf(buffcal, "%f\t%f\t%f\t%f\n", d, m->filt_ax, m->filt_ay, m->filt_az);
            //HAL_UART_Transmit(&huart2, (uint8_t*) buffcal, strlen(buffcal), 100);

            if(d < 3000){
                valid = false; break;
            } 
        }

        if(valid){
            #if PORT == LED 
                HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            #endif
            acc[head][0] = m->raw_ax, acc[head][1] = m->raw_ay, acc[head][2] = m->raw_az;
            head++, cnt++, head%= tot; 
        }
        setReg(CAL_ACC, 100*fmin(cnt,6)/7 );

        if(cnt >= tot && valid){

            for(int i = 1 ; i <= neq-1; i++){
                aux = 0;
                for(int j = 0; j < 3; j++){
                    setMatVal(&A, i-1, j, 2*(acc[(head + i-1)%6][j] - acc[(head + i)%6][j]));
                    //x2*x2-x1*x1+y2*y2-y1*y1+z2*z2-z1*z1 = 2*c1*(x1-x2)+2*c2*(y1-y2)+2*c3*(z1-z2)
                    aux += acc[(head + i)%6][j]*acc[(head + i)%6][j] - acc[(head + i-1)%6][j]*acc[(head + i-1)%6][j]; 
                }
                setMatVal(&b, i-1, 0, aux);
            }

            gaussElimination3x3(&A, &b, &ans);
            aux = 0;
            for(int i = 0; i < 3; i++){
                aux += (acc[head][i] + getMatVal(&ans, i, 0))*(acc[head][i] + getMatVal(&ans, i, 0));
            }
            scale = sqrt(aux/G/G);

            valid = true;
            for(int i = neq ; i <= tot-1; i++){
                aux = 0;
                for(int j= 0; j < 3; j++){
                    aux += (acc[(head+i)%tot][j] + getMatVal(&ans, j, 0))*(acc[(head+i)%tot][j] + getMatVal(&ans, j, 0));
                }
                if(fabs(sqrt(aux/scale/scale) - G) > 0.3){
                    //Serial.println(sqrt(aux/scale/scale));
                    valid = false;
                    break;
                }
            }
            if(valid) done = true;

        }
    }

    m -> off_ax = getMatVal(&ans, 0, 0);
    m -> off_ay = getMatVal(&ans, 1, 0);
    m -> off_az = getMatVal(&ans, 2, 0);
    m -> scl_acc = scale;

    setReg(CAL_ACC, 100);
    setReg(ACC_X_OFF, m->off_ax );    
    setReg(ACC_Y_OFF, m->off_ay );
    setReg(ACC_Z_OFF, m->off_az );
    setReg(ACC_SCALE, m->scl_acc);  
/*
    char buff[50];

    sprintf(buff, "%f %f %f %f\n", m->off_ax, m->off_ay, m->off_az, m->scl_acc);
    HAL_UART_Transmit(&huart1, (uint8_t *)buff, strlen(buff), 100);*/

    matDestruct(&A);
    matDestruct(&b);
    matDestruct(&ans);
}

/*
void calibrateIcmMag(icm20948* m){
    
    //char aux_buff[50]="";

    setReg(CAL_MAG,0);

    int head = 0, cnt = 0;
    bool done = false, valid;
    const int n = 100;
    float magX, magY, magZ, scaleGlobal = 0.01;
    float mag[n][3];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < 3; j++){
            mag[i][j] = 0;
        }    
    }

    while (!done){
        HAL_Delay(100);
        readIcmRawMag(m);
        magX = m->raw_mx*scaleGlobal;
        magY = m->raw_my*scaleGlobal;
        magZ = m->raw_mz*scaleGlobal;
        
        //sprintf(aux_buff, "%f %f %f\n", m->raw_mx, m->raw_my, m->raw_mz);
        //HAL_UART_Transmit(&huart2, (uint8_t*) aux_buff, strlen(aux_buff), 100);
        valid = true;
        for(int i = 1 ; i <= cnt ; i++){
            int j = (head - i + n) % n;
            float d = dis3d(magX, magY, magZ, mag[j][0], mag[j][1], mag[j][2]);

            //sprintf(aux_buff, "%f \n", d);
            //HAL_UART_Transmit(&huart2, (uint8_t*) aux_buff, strlen(aux_buff), 100);

            if(d < 20*scaleGlobal){
                valid = false; break;
            } 
        }
        if(valid){
            #if PORT == LED 
                HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            #endif
            mag[head][0] = magX, mag[head][1] = magY, mag[head][2] = magZ;
            head++, cnt++, head%= n; 
        }
        setReg(CAL_MAG, fmin(cnt, 99.0));
        
        //sprintf(aux_buff, "%d\n", cnt);
        //HAL_UART_Transmit(&huart2, (uint8_t*) aux_buff, strlen(aux_buff), 100);

        if(cnt == n){done = true;}
    }

    mat H, Ht, w, prod, prod2, X, inverse;
    matInit(&H, n, 6);
    matInit(&Ht, 6, n);
    matInit(&prod, 6, 6);
    matInit(&prod2, 6, n);
    matInit(&inverse, 6, 6);
    matInit(&w, n, 1);
    matInit(&X, 6, 1);


    for(int i = 0; i < n; i++){
        setMatVal(&H, i, 0, mag[i][0]);
        setMatVal(&H, i, 1, mag[i][1]);
        setMatVal(&H, i, 2, mag[i][2]);
        setMatVal(&H, i, 3, -mag[i][1]*mag[i][1]);
        setMatVal(&H, i, 4, -mag[i][2]*mag[i][2]);
        setMatVal(&H, i, 5, 1);

        setMatVal(&w, i, 0, mag[i][0]*mag[i][0]);
    }
    matTrans(&Ht, &H);
    matMult(&prod, &Ht, &H);
    
    matInverse(&inverse, &prod);
    matMult(&prod2, &inverse, &Ht);
    matMult(&X, &prod2, &w);
    
    m -> off_mx = getMatVal(&X, 0, 0)/(2);
    m -> off_my = getMatVal(&X, 1, 0)/(2*getMatVal(&X, 3, 0));
    m -> off_mz = getMatVal(&X, 2, 0)/(2*getMatVal(&X, 4, 0));
    
    float temp = getMatVal(&X, 5, 0) + (m -> off_mx) * (m -> off_mx) + (m -> off_my) * (m -> off_my) + (m -> off_mz) * (m -> off_mz);
    
    m -> off_mx /= scaleGlobal;
    m -> off_my /= scaleGlobal;
    m -> off_mz /= scaleGlobal;
    
    m -> scl_magx = sqrt(temp)/ (scaleGlobal);
    m -> scl_magy = sqrt(temp / getMatVal(&X, 3, 0)) / (scaleGlobal);
    m -> scl_magz = sqrt(temp / getMatVal(&X, 4, 0)) / (scaleGlobal);

    setReg( MAG_X_OFF ,m -> off_mx);
    setReg( MAG_Y_OFF ,m -> off_my);
    setReg( MAG_Z_OFF ,m -> off_mz);
    setReg( MAG_X_SCALE ,m -> scl_magx);
    setReg( MAG_Y_SCALE ,m -> scl_magy);
    setReg( MAG_Z_SCALE ,m -> scl_magy);
    setReg( CAL_MAG, 100);
    
    matDestruct(&H);
    matDestruct(&Ht);
    matDestruct(&prod);    
    matDestruct(&prod2); 
    matDestruct(&inverse); 
    matDestruct(&w); 
    matDestruct(&X); 

}*/