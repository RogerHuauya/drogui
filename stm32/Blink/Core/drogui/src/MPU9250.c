#include "MPU9250.h"
#include "i2c.h"
#include "matlib.h"
#include "registerMap.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

#define G 9.81
int16_t _ax, _ay, _az, _gx, _gy, _gz, _mx, _my, _mz;

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data){
    
    HAL_I2C_Master_Transmit(&hi2c1, (Address << 1), &Register, 1, 100);
    HAL_I2C_Master_Receive(&hi2c1, (Address << 1) | 1, Data, Nbytes, 100);
}
 
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data){

    HAL_I2C_Master_Transmit(&hi2c1, (Address << 1), &Register, 1, 100);
    HAL_I2C_Master_Transmit(&hi2c1, (Address << 1), &Data, 1, 100);
}



void initMpu(mpu9250* m){

    I2CwriteByte(MPU9250_ADDRESS, MASTER_CONFIG, 0x06);
    I2CwriteByte(MPU9250_ADDRESS, GYRO_CONFIG, GYRO_FULL_SCALE_2000_DPS | 3);
    I2CwriteByte(MPU9250_ADDRESS, ACCEL_CONFIG1, ACC_FULL_SCALE_8_G);
    I2CwriteByte(MPU9250_ADDRESS, ACCEL_CONFIG2, 4);
    I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);
    I2CwriteByte(MAG_ADDRESS, 0x0A, 0x16);
    m->scl_acc = m->scl_magx = m-> scl_magy = m-> scl_magz = 1;
    
}

void readRawAcc(mpu9250* m){ // m/s^2
    uint8_t Buf[6];
    I2Cread(MPU9250_ADDRESS, 0x3B, 6, Buf);
    _ax = -((Buf[0]<<8) | Buf[1]);
    _ay = -((Buf[2]<<8) | Buf[3]);
    _az =   (Buf[4]<<8) | Buf[5];
    /*
    char buff[50];

    for(int i = 0; i < 6 ; i++){

        sprintf(buff, "%x\t", Buf[i]);
        HAL_UART_Transmit(&huart1, (uint8_t *)buff, strlen(buff), 100);
    }

    sprintf(buff, "\n");
    HAL_UART_Transmit(&huart1, (uint8_t *)buff, strlen(buff), 100);*/
    m -> raw_ax = _ax;
    m -> raw_ay = _ay;
    m -> raw_az = _az; 
}

void readRawGyro(mpu9250* m){ // degrees/sec
    
    uint8_t Buf[6];
    I2Cread(MPU9250_ADDRESS, 0x43, 6, Buf);
    _gx = -((Buf[0] << 8) | Buf[1]);
    _gy = -((Buf[2] << 8) | Buf[3]);
    _gz =   (Buf[4] << 8) | Buf[5];
    m -> raw_gx = _gx;
    m -> raw_gy = _gy;
    m -> raw_gz = _gz; 
}

void readRawMag(mpu9250* m){ // m/s^2
    uint8_t Buf[7];
    I2Cread(MAG_ADDRESS, 0x03, 7, Buf);
    _mx = -((Buf[3]<<8) | Buf[2]);
    _my = -((Buf[1]<<8) | Buf[0]);
    _mz = -((Buf[5]<<8) | Buf[4]);
    m -> raw_mx = _mx;
    m -> raw_my = _my;
    m -> raw_mz = _mz; 
}

void readAcc(mpu9250* m){ // m/s^2
    uint8_t Buf[6];
    I2Cread(MPU9250_ADDRESS, 0x3B, 6, Buf);
    _ax = -((Buf[0]<<8) | Buf[1]);
    _ay = -((Buf[2]<<8) | Buf[3]);
    _az =   (Buf[4]<<8) | Buf[5];
    m -> ax = (_ax + m->off_ax)/m->scl_acc;
    m -> ay = (_ay + m->off_ay)/m->scl_acc;
    m -> az = (_az + m->off_az)/m->scl_acc; 
}


void readGyro(mpu9250* m){ // degrees/sec
    
    uint8_t Buf[6];
    I2Cread(MPU9250_ADDRESS, 0x43, 6, Buf);
    _gx = -((Buf[0] << 8) | Buf[1]);
    _gy = -((Buf[2] << 8) | Buf[3]);
    _gz =   (Buf[4] << 8) | Buf[5];
    
    m -> gx = _gx + m->off_gx;
    m -> gy = _gy + m->off_gy;
    m -> gz = _gz + m->off_gz; 
}

void readMag(mpu9250* m){ // m/s^2
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

// 120 260 380

bool quiet(mpu9250* m, int n, float treshold, bool cal){
    float max_gyro[3] = {0,0,0};
    float min_gyro[3] = {0,0,0};
    
    for(int i = 0; i < n; i++){
        
        readRawGyro(m);   /*
        Serial.print(m->gx);
        Serial.print("\t");
        Serial.print(m->gy);
        Serial.print("\t");
        Serial.print(m->gz);
        Serial.print("\n");*/

        if(i == 0){
            max_gyro[0] = m->raw_gx, max_gyro[1] = m->raw_gy, max_gyro[2] = m->raw_gz;
            min_gyro[0] = m->raw_gx, min_gyro[1] = m->raw_gy, min_gyro[2] = m->raw_gz;
        }
        else{
                max_gyro[0] = fmax(max_gyro[0], m->raw_gx);
                max_gyro[1] = fmax(max_gyro[1], m->raw_gy);
                max_gyro[2] = fmax(max_gyro[2], m->raw_gz);
                
                min_gyro[0] = fmin(min_gyro[0], m->raw_gx);
                min_gyro[1] = fmin(min_gyro[1], m->raw_gy);
                min_gyro[2] = fmin(min_gyro[2], m->raw_gz);
        }
        HAL_Delay(2);
    }
    /*
    for(int i = 0 ; i < 3; i++){
        Serial.print(max_gyro[i] -min_gyro[i]);
        Serial.print("\t");
    }
    Serial.println();*/
    
   //120 260 380
    if((max_gyro[0]-min_gyro[0] < (treshold+220)) && (max_gyro[1]-min_gyro[1] < (treshold+400)) && (max_gyro[2]-min_gyro[2] < (treshold+380))){
        if(cal){
            //Serial.print("Ra");
            m->off_gx = -(max_gyro[0] + min_gyro[0])/2;
            m->off_gy = -(max_gyro[1] + min_gyro[1])/2;
            m->off_gz = -(max_gyro[2] + min_gyro[2])/2;
        }
        return true;
    }
    return false;
}
void calibrateGyro(mpu9250* m){
    
    setReg(CAL_GYR,0);
    while(!quiet(m,200,0, true));
    setReg(CAL_GYR, 100);
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
}

float dis3d(float x,float y,float z, float a, float b, float c){
    return sqrt((x - a)*(x - a) + (y - b)*(y - b) + (z - c)*(z - c));
}
void calibrateAccel(mpu9250* m){
    
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
        while(!quiet(m,100,30, false));
        readRawAcc(m);
        valid = true;
        for(int i = 1 ; i <= tot-1 ; i++){
            int j = (head - i + tot) % tot;
            // Serial.println("quiet ");
            if(dis3d(m->raw_ax, m->raw_ay, m->raw_az, acc[j][0],acc[j][1],acc[j][2]) < 1000){
                valid = false; break;
            } 
        }

        if(valid){
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
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
/*
    char buff[50];

    sprintf(buff, "%f %f %f %f\n", m->off_ax, m->off_ay, m->off_az, m->scl_acc);
    HAL_UART_Transmit(&huart1, (uint8_t *)buff, strlen(buff), 100);*/

    matDestruct(&A);
    matDestruct(&b);
    matDestruct(&ans);
}

void calibrateMag(mpu9250* m){
    
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
        readRawMag(m);
        magX = m->raw_mx*scaleGlobal;
        magY = m->raw_my*scaleGlobal;
        magZ = m->raw_mz*scaleGlobal;

        valid = true;
        for(int i = 1 ; i <= cnt ; i++){
            int j = (head - i + n) % n;
            if(dis3d(magX, magY, magZ, mag[j][0], mag[j][1], mag[j][2]) < 20*scaleGlobal){
                valid = false; break;
            } 
        }
        if(valid){
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            mag[head][0] = magX, mag[head][1] = magY, mag[head][2] = magZ;
            head++, cnt++, head%= n; 
        }
        setReg(CAL_MAG, cnt);
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
/*
    Serial.print("mx = [ ");
    for(int i = 0 ; i < 99 ; i++){
        Serial.print(mag[i][0]), Serial.print(";");
    }
    Serial.print(mag[99][0]), Serial.println("];");

    Serial.print("my = [ ");
    for(int i = 0 ; i < 99 ; i++){
        Serial.print(mag[i][1]), Serial.print(";");
    }
    Serial.print(mag[99][1]), Serial.println("];");
    
    
    Serial.print("mz = [ ");
    for(int i = 0 ; i < 99 ; i++){
        Serial.print(mag[i][2]), Serial.print(";");
    }
    Serial.print(mag[99][2]), Serial.println("];");*/
    


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
    
    
    /*
    Serial.print("H = [");
    for(int i = 0; i < prod.row; i++){
        for(int j = 0; j < prod.col; j++){
            Serial.print(" ");
            dtostrf(getMatVal(&prod, i, j), 15, 2, str_temp);
            Serial.println(str_temp);

        }
        Serial.println(";");
    }
    Serial.println("];");
    */
    matInverse(&inverse, &prod);
    matMult(&prod2, &inverse, &Ht);
    matMult(&X, &prod2, &w);
    /*
    for(int i = 0; i < 6; i++)
        Serial.println(getMatVal(&X, i, 0));
    */
    m -> off_mx = getMatVal(&X, 0, 0)/(2);
    m -> off_my = getMatVal(&X, 1, 0)/(2*getMatVal(&X, 3, 0));
    m -> off_mz = getMatVal(&X, 2, 0)/(2*getMatVal(&X, 4, 0));
    
    float temp = getMatVal(&X, 5, 0) + (m -> off_mx) * (m -> off_mx) + (m -> off_my) * (m -> off_my) + (m -> off_mz) * (m -> off_mz);
    //Serial.println(temp);
    
    m -> off_mx /= scaleGlobal;
    m -> off_my /= scaleGlobal;
    m -> off_mz /= scaleGlobal;
    
    m -> scl_magx = sqrt(temp)/ (scaleGlobal);
    m -> scl_magy = sqrt(temp / getMatVal(&X, 3, 0)) / (scaleGlobal);
    m -> scl_magz = sqrt(temp / getMatVal(&X, 4, 0)) / (scaleGlobal);
/*
    Serial.print(m -> off_mx);
    Serial.print("\t");
    Serial.print(m -> off_my);
    Serial.print("\t");
    Serial.print(m -> off_mz);
    Serial.print("\t");
    Serial.println(m -> scl_magx);
    Serial.print("\t");
    Serial.println(m -> scl_magy);
    Serial.print("\t");
    Serial.println(m -> scl_magz);*/
    
    matDestruct(&H);
    matDestruct(&Ht);
    matDestruct(&prod);    
    matDestruct(&prod2); 
    matDestruct(&inverse); 
    matDestruct(&w); 
    matDestruct(&X); 
/*
    Serial.print("mxf = [ ");
    for(int i = 0 ; i < 99 ; i++){
        Serial.print((mag[i][0] - m->off_mx)/m->scl_magx), Serial.print(";");
    }
    Serial.print((mag[99][0] - m->off_mx)/m->scl_magx), Serial.println("];");

    Serial.print("myf = [ ");
    for(int i = 0 ; i < 99 ; i++){
        Serial.print((mag[i][1] - m->off_my)/m->scl_magy), Serial.print(";");
    }
    Serial.print((mag[99][1] - m->off_my)/m->scl_magy), Serial.println("];");

    Serial.print("mzf = [ ");
    for(int i = 0 ; i < 99 ; i++){
        Serial.print((mag[i][2] - m->off_mz)/m->scl_magz), Serial.print(";");
    }
    Serial.print((mag[99][2] - m->off_mz)/m->scl_magz), Serial.println("];");*/

       
}