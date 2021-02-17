#include "..\headers\sensorsTasks.h"

#define N_BMP 25

Adafruit_BMP3XX bmp;


mpu9250 myIMU;

float alt_memo[N_BMP], alt_fast, alt_slow = 0, alt_diff, sum = 0,alt_offs;
int alt_pointer = 0;
float sealevel;

filter filter_gx, filter_gy, filter_gz;
timer timer_accel, timer_gyro, timer_mag;

float roll, pitch, yaw, ax, ay, az, gx, gy, gz, x, y, z;


void accelInterrupt(){
    readAcc(&myIMU);
    setReg(ACC_X,(float)(myIMU.ax));
    setReg(ACC_Y,(float)(myIMU.ay));
    setReg(ACC_Z,(float)(myIMU.az));
    ax = (myIMU.ax);
    ay = (myIMU.ay);
    az = (myIMU.az);
}

void gyroInterrupt(){
    readGyro(&myIMU);
    setReg(GYRO_X, myIMU.gx);
    setReg(GYRO_Y, myIMU.gy);
    setReg(GYRO_Z, myIMU.gz);
    gx = computeFilter(&filter_gx, myIMU.gx)/100.0;
    gy = computeFilter(&filter_gy, myIMU.gy)/100.0;
    gz = computeFilter(&filter_gz, myIMU.gz)/100.0;
}

void magInterrupt(){
    readMag(&myIMU);
}

void positionInterrupt(){
    

    if(getReg(START) > 0){
        kalmanUpdateIMU(ax, ay, az, roll, pitch, yaw);
        if(getReg(GPS_AVAILABLE) == 1) setReg(GPS_AVAILABLE, 0), kalmanUpdateGPS(getReg(GPS_X), getReg(GPS_Y), 0);    
    }
    else{
        clearKalman();
    }

    getPosition(&x, &y, &z);
    
    z = alt_slow - alt_offs;

    setReg(X_VAL, x);
    setReg(Y_VAL, y);
    setReg(Z_VAL, z);  
}


void bmpInterrupt(){

    alt_pointer %= N_BMP;
    sum -= alt_memo[alt_pointer];
    alt_memo[alt_pointer] = bmp.readAltitude(sealevel);
    sum += alt_memo[alt_pointer++];
    alt_fast = sum / N_BMP;

    alt_slow = alt_slow * 0.99 + alt_fast * 0.01;

    alt_diff = alt_fast - alt_slow;
    alt_diff = max(min(alt_diff, 1), -1);

    if (abs(alt_diff) >  0.20) alt_slow += alt_diff / 6.0;

    if(security) alt_offs = alt_slow;

}

void initSensorsTasks(){
    
    initMpu(&myIMU);
    
    setKalmanTsImu(0.01);
    setKalmanTsGps(1);
    initMatGlobal();

    initFilter(&filter_gx, 10 , coeffA_150Hz, coeffB_150Hz);
    initFilter(&filter_gy, 10 , coeffA_150Hz, coeffB_150Hz);
    initFilter(&filter_gz, 10 , coeffA_150Hz, coeffB_150Hz);

    calibrateGyro(&myIMU);
    Serial.println("Gyro calibrated ...!!");
    
    calibrateAccel(&myIMU);
    Serial.println("Accel calibrated ...!!");
    /*
    calibrateMag(&myIMU);
    Serial.println("Mag calibrated ...!!");*/
    

    initTimer(&timer_accel, &accelInterrupt, 1000);
    initTimer(&timer_gyro, &gyroInterrupt, 1000);
    initTimer(&timer_mag, &magInterrupt, 10);
}

void executeSensorsTasks(){
    if(timerReady(&timer_accel))  executeTimer(&timer_accel);
    if(timerReady(&timer_gyro))  executeTimer(&timer_gyro);  
    if(timerReady(&timer_mag))  executeTimer(&timer_mag);  
}