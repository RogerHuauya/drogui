#include "../headers/MPU9250.h"
#include <Wire.h>
#include "../headers/matlib.h"
#define G 9.81
int16_t _ax, _ay, _az, _gx, _gy, _gz, _mx, _my, _mz;

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data){
    
    Wire.beginTransmission(Address);
    Wire.write(Register);
    Wire.endTransmission();
    
    Wire.requestFrom(Address, Nbytes); 
    uint8_t index=0;
    while (Wire.available())
    Data[index++]=Wire.read();
}
 
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data){

    Wire.beginTransmission(Address);
    Wire.write(Register);
    Wire.write(Data);
    Wire.endTransmission();
}



void initMpu(mpu9250* m){
    Wire.begin();
    Wire.setClock(100000);
    delay(10);
    I2CwriteByte(MPU9250_ADDRESS, MASTER_CONFIG, 0x06);
    delay(10);
    I2CwriteByte(MPU9250_ADDRESS, GYRO_CONFIG, GYRO_FULL_SCALE_2000_DPS | 3);
    delay(10);
    I2CwriteByte(MPU9250_ADDRESS, ACCEL_CONFIG1, ACC_FULL_SCALE_8_G);
    delay(10);
    I2CwriteByte(MPU9250_ADDRESS, ACCEL_CONFIG2, 4);
    delay(10);
    I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);
    delay(10);
    I2CwriteByte(MAG_ADDRESS, 0x0A, 0x16);
    delay(10);
    
}

void readRawAcc(mpu9250* m){ // m/s^2
    uint8_t Buf[6];
    I2Cread(MPU9250_ADDRESS, 0x3B, 6, Buf);
    _ax = -((Buf[0]<<8) | Buf[1]);
    _ay = -((Buf[2]<<8) | Buf[3]);
    _az =   (Buf[4]<<8) | Buf[5];
    m -> raw_ax = _ax;
    m -> raw_ay = _ay;
    m -> raw_az = _az; 
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

    m -> mx = (_mx + m->off_mx)/m->scl_mag;
    m -> my = (_my + m->off_my)/m->scl_mag;
    m -> mz = (_mz + m->off_mz)/m->scl_mag; 
}

// 120 260 380

bool quiet(mpu9250* m, int n, float treshold, bool cal = false){
    float max_gyro[3] = {0,0,0};
    float min_gyro[3] = {0,0,0};
    
    for(int i = 0; i < n; i++){
        
        readGyro(m);   /*
        Serial.print(m->gx);
        Serial.print("\t");
        Serial.print(m->gy);
        Serial.print("\t");
        Serial.print(m->gz);
        Serial.print("\n");*/
        if(i == 0){
            max_gyro[0] = m->gx, max_gyro[1] = m->gy, max_gyro[2] = m->gz;
            min_gyro[0] = m->gx, min_gyro[1] = m->gy, min_gyro[2] = m->gz;
        }
        else{
                max_gyro[0] = max(max_gyro[0], m->gx);
                max_gyro[1] = max(max_gyro[1], m->gy);
                max_gyro[2] = max(max_gyro[2], m->gz);
                
                min_gyro[0] = min(min_gyro[0], m->gx);
                min_gyro[1] = min(min_gyro[1], m->gy);
                min_gyro[2] = min(min_gyro[2], m->gz);
        }
        delay(2);
    }
    /*
    for(int i = 0 ; i < 3; i++){
        Serial.print(max_gyro[i] -min_gyro[i]);
        Serial.print("\t");
    }
    Serial.println();*/
    if((max_gyro[0]-min_gyro[0] < (treshold+120)) && (max_gyro[1]-min_gyro[1] < (treshold+260)) && (max_gyro[2]-min_gyro[2] < (treshold+380))){
        if(cal){

            m->off_gx = -(max_gyro[0] + min_gyro[0])/2;
            m->off_gy = -(max_gyro[1] + min_gyro[1])/2;
            m->off_gz = -(max_gyro[2] + min_gyro[2])/2;
        }
        return true;
    }
    return false;
}
void calibrateGyro(mpu9250* m){
    while(!quiet(m,100,0, true));
}

float dis3d(float x,float y,float z, float a, float b, float c){
    return sqrt((x - a)*(x - a) + (y - b)*(y - b) + (z - c)*(z - c));
}
void calibrateAccel(mpu9250* m){
    
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
        while(!quiet(m,100,30));
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
            Serial.println("muestra válida");
            acc[head][0] = m->raw_ax, acc[head][1] = m->raw_ay, acc[head][2] = m->raw_az;
            head++, cnt++, head%= tot; 
        }
        

        if(cnt >= tot && valid){
            Serial.println("six");
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
                    Serial.println(sqrt(aux/scale/scale));
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

    matDestruct(&A);
    matDestruct(&b);
    matDestruct(&ans);
}

void calibrateMag(mpu9250* m){
    int neq = 4, nval = 6;
    int tot = neq + nval;

    float mag[tot][3], scale, aux;
    for(int i = 0; i < tot; i++){
        for(int j = 0; j < 3; j++){
            mag[i][j] = 0;
        }    
    }

    mat A, b, ans;
    matInit(&A,3,3);
    matInit(&b,3,1);
    matInit(&ans,3,1);

    int head = 0, cnt = 0;
    bool done = false, valid;
    
    while(!done){

        readRawMag(m);
        valid = true;
        for(int i = 1 ; i <= tot-1 ; i++){
            int j = (head - i + tot) % tot;
            float d = dis3d(m->raw_mx, m->raw_my, m->raw_mz, mag[j][0], mag[j][1], mag[j][2]);
            //Serial.println(d);
            if(d < 80){
                valid = false; break;
            } 
        }

        

        if(valid){
            Serial.println("muestra válida");
            mag[head][0] = m->raw_mx, mag[head][1] = m->raw_my, mag[head][2] = m->raw_mz;
            head++, cnt++, head%= tot; 
        }
        

        if(cnt >= tot && valid){
            Serial.println("six");
            for(int i = 1 ; i <= neq-1; i++){
                aux = 0;
                for(int j = 0; j < 3; j++){
                    setMatVal(&A, i-1, j, 2*(mag[(head + i-1)%6][j] - mag[(head + i)%tot][j]));
                    //x2*x2-x1*x1+y2*y2-y1*y1+z2*z2-z1*z1 = 2*c1*(x1-x2)+2*c2*(y1-y2)+2*c3*(z1-z2)
                    aux += mag[(head + i)%tot][j]*mag[(head + i)%tot][j] - mag[(head + i-1)%tot][j]*mag[(head + i-1)%tot][j]; 
                }
                setMatVal(&b, i-1, 0, aux);
            }

            gaussElimination3x3(&A, &b, &ans);
            aux = 0;
            for(int i = 0; i < 3; i++){
                aux += (mag[head][i] + getMatVal(&ans, i, 0))*(mag[head][i] + getMatVal(&ans, i, 0));
            }
            
            scale = sqrt(aux);

            valid = true;
            
            for(int i = neq ; i <= tot-1; i++){
                aux = 0;
                for(int j= 0; j < 3; j++){
                    aux += (mag[(head+i)%tot][j] + getMatVal(&ans, j, 0))*(mag[(head+i)%tot][j] + getMatVal(&ans, j, 0));
                }
                if(fabs(sqrt(aux/scale/scale) - 1) > 0.2){
                    Serial.println(sqrt(aux/scale/scale));
                    valid = false;
                    break;
                }
            }
            if(valid) done = true;

        }
        delay(10);
        
    }

    m -> off_mx = getMatVal(&ans, 0, 0);
    m -> off_my = getMatVal(&ans, 1, 0);
    m -> off_mz = getMatVal(&ans, 2, 0);
    m -> scl_mag = scale;

    Serial.print(m -> off_mx);
    Serial.print("\t");
    Serial.print(m -> off_my);
    Serial.print("\t");
    Serial.print(m -> off_mz);
    Serial.print("\t");
    Serial.println(m -> scl_mag);
    
    matDestruct(&A);
    matDestruct(&b);
    matDestruct(&ans);    
}