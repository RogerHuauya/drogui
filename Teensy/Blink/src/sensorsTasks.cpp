#include "..\headers\sensorsTasks.h"

#define N_BMP 25

Adafruit_BMP3XX bmp;


mpu9250 myIMU;

float alt_memo[N_BMP], alt_fast, alt_slow = 0, alt_diff, sum = 0,alt_offs;
int alt_pointer = 0;
float sealevel;

filter filter_gx, filter_gy, filter_gz;
filter filter_gx2, filter_gy2, filter_gz2;
timer timer_accel, timer_gyro, timer_mag;

float roll, pitch, yaw, ax, ay, az, gx, gy, gz, x, y, z;


void accelInterrupt(){
    readAcc(&myIMU);
    ax = (myIMU.ax);
    ay = (myIMU.ay);
    az = (myIMU.az);
    setReg(ACC_X,(float)(ax));
    setReg(ACC_Y,(float)(ay));
    setReg(ACC_Z,(float)(az));
}

void gyroInterrupt(){
    readGyro(&myIMU);
    
    int quanti = 10;

    gx = computeFilter(&filter_gx2, myIMU.gx);
    gy = computeFilter(&filter_gy2, myIMU.gy);
    gz = computeFilter(&filter_gz2, myIMU.gz);

    gx = quanti*((int) gx/(5*quanti));
    gy = quanti*((int) gy/(5*quanti));
    gz = quanti*((int) gz/(5*quanti));

    gx = computeFilter(&filter_gx, gx);
    gy = computeFilter(&filter_gy, gy);
    gz = computeFilter(&filter_gz, gz);
    

    setReg(GYRO_X, gx);
    setReg(GYRO_Y, gy);
    setReg(GYRO_Z, gz);
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

    initFilter(&filter_gx, 9 , coeffA_100Hz, coeffB_100Hz);
    initFilter(&filter_gy, 9 , coeffA_100Hz, coeffB_100Hz);
    initFilter(&filter_gz, 9 , coeffA_100Hz, coeffB_100Hz);

    initFilter(&filter_gx2, 11 , coeffA_300Hz, coeffB_300Hz);
    initFilter(&filter_gy2, 11 , coeffA_300Hz, coeffB_300Hz);
    initFilter(&filter_gz2, 11 , coeffA_300Hz, coeffB_300Hz);
    
    calibrateGyro(&myIMU);
    //calibrateAccel(&myIMU);
    /*
    calibrateMag(&myIMU);
    Serial.println("Mag calibrated ...!!");*/
    

    //initTimer(&timer_accel, &accelInterrupt, 1000);
    initTimer(&timer_gyro, &gyroInterrupt, 1000);
    initTimer(&timer_mag, &magInterrupt, 10);
}

void executeSensorsTasks(){
    //if(timerReady(&timer_accel))  executeTimer(&timer_accel);
    if(timerReady(&timer_gyro))  executeTimer(&timer_gyro);  
    if(timerReady(&timer_mag))  executeTimer(&timer_mag);  
}