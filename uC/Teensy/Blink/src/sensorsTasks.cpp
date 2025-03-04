#include "..\headers\sensorsTasks.h"
#include "..\headers\mahony.h"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Arduino.h>
#define N_BMP 25

Adafruit_BMP3XX bmp;


mpu9250 myIMU;
SFE_UBLOX_GNSS myGNSS;

float alt_memo[N_BMP], alt_fast, alt_slow = 0, alt_diff, sum = 0,alt_offs;
int alt_pointer = 0;
float sealevel;
char gpsData[80];
filter filter_gx, filter_gy, filter_gz;
filter filter_gx2, filter_gy2, filter_gz2;
dNotchFilter dnotch_gx, dnotch_gy, dnotch_gz; 
dNotchFilter dnotch_gx2, dnotch_gy2, dnotch_gz2;

filter filter_roll, filter_pitch, filter_yaw;

filter filter_ax, filter_ay, filter_az;
dNotchFilter dnotch_ax, dnotch_ay, dnotch_az; 


timer timer_accel, timer_gyro, timer_mag, timer_rpy, timer_gps, timer_pos;
float roll, pitch, yaw, ax, ay, az, gx, gy, gz, mx, my, mz, x, y, z, xi, yi;
long lat, lon;
void initGPS(){

    do {
    
        Serial.println("GPS: trying 38400 baud");
        Serial1.begin(38400);
        if (myGNSS.begin(Serial1) == true) break;

        delay(100);
        
        Serial.println("GPS: trying 9600 baud");
        Serial1.begin(9600);
        if (myGNSS.begin(Serial1) == true) {
            Serial.println("GPS: connected at 9600 baud, switching to 38400");
            myGNSS.setSerialRate(38400);
            delay(100);
        } else {
            //myGNSS.factoryReset();
            delay(2000); //Wait a bit before trying again to limit the Serial output
        }
    } while(1);
    Serial.println("GPS serial connected");

    myGNSS.setUART1Output(COM_TYPE_UBX); //Set the UART port to output UBX only
    myGNSS.setNavigationFrequency(2); //Produce two solutions per second
    myGNSS.setAutoPVT(true); //Tell the GNSS to "send" each solution
    myGNSS.saveConfiguration(); //Save the current settings to flash and BBR
    delay(1000);

    while( !myGNSS.getPVT()){}
    xi = myGNSS.getLatitude()*1e-7, yi = myGNSS.getLongitude()*1e-7;

}
void accelInterrupt(){
    readAcc(&myIMU);
    
    ax = computeFilter(&filter_ax, myIMU.ax);
    ay = computeFilter(&filter_ay, myIMU.ay);
    az = computeFilter(&filter_az, myIMU.az);
    /*
    ax = myIMU.ax;
    ay = myIMU.ay;
    az = myIMU.az;*/

    
    ax = computeDNotch(&dnotch_ax, ax);
    ay = computeDNotch(&dnotch_ay, ay);
    az = computeDNotch(&dnotch_az, az);

    setReg(ACC_X,(float)(ax));
    setReg(ACC_Y,(float)(ay));
    setReg(ACC_Z,(float)(az));
}

void gyroInterrupt(){
    readGyro(&myIMU);
    
    int quanti = 1;

    gx = computeFilter(&filter_gx, myIMU.gx);
    gy = computeFilter(&filter_gy, myIMU.gy);
    gz = computeFilter(&filter_gz, myIMU.gz);

    
    gx = computeFilter(&filter_gx2, gx);
    gy = computeFilter(&filter_gy2, gy);
    gz = computeFilter(&filter_gz2, gz);
    

    gx = computeDNotch(&dnotch_gx, gx);
    gy = computeDNotch(&dnotch_gy, gy);
    gz = computeDNotch(&dnotch_gz, gz);

    gx = computeDNotch(&dnotch_gx2, gx);
    gy = computeDNotch(&dnotch_gy2, gy);
    gz = computeDNotch(&dnotch_gz2, gz);
    /*
    gx = myIMU.gx;
    gy = myIMU.gy;
    gz = myIMU.gz;*/

    gx /= 5;
    gy /= 5;
    gz /= 5; 


    setReg(GYRO_X, gx);
    setReg(GYRO_Y, gy);
    setReg(GYRO_Z, gz);
}

void magInterrupt(){
    readMag(&myIMU);
    mx = myIMU.mx;
    my = myIMU.my;
    mz = myIMU.mz;
}
float Kdfilt = 1000;
void rpyInterrupt(){
    float rpy[3];
    
    mahonyUpdate(gx*PI/360.0f, gy*PI/360.0f, gz*PI/360.0f, ax, ay, az, my, mx, mz);
    getMahonyEuler(rpy);
    roll = rpy[0], pitch = rpy[1], yaw = rpy[2];
    /*
    roll += max(min(Kdfilt, (rpy[0] - roll)),-Kdfilt);
    pitch += max(min(Kdfilt, (rpy[1] - pitch)),-Kdfilt);
    yaw += max(min(Kdfilt,(rpy[2] - yaw)),-Kdfilt);
    */
    /*roll = computeFilter(&filter_roll, roll);
    pitch = computeFilter(&filter_pitch, pitch);
    yaw = computeFilter(&filter_yaw, yaw);*/

    setReg(ROLL_VAL, roll);
    setReg(PITCH_VAL, pitch);
    setReg(YAW_VAL, yaw);
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
    //alt_memo[alt_pointer] = bmp.readAltitude(sealevel);
    sum += alt_memo[alt_pointer++];
    alt_fast = sum / N_BMP;

    alt_slow = alt_slow * 0.99 + alt_fast * 0.01;

    alt_diff = alt_fast - alt_slow;
    alt_diff = max(min(alt_diff, 1), -1);

    if (abs(alt_diff) >  0.20) alt_slow += alt_diff / 6.0;

    if(security) alt_offs = alt_slow;

}

void gpsInterrupt(){
    if (myGNSS.getPVT()){
        lat = myGNSS.getLatitude();
        lon = myGNSS.getLongitude();
        setReg(GPS_X, 100000*(lat*1e-7 - xi));
        setReg(GPS_Y, 100000*(lon*1e-7 - yi));
        setReg(GPS_AVAILABLE, 1);
    }

}

void initSensorsTasks(){
    
    initMpu(&myIMU);
    initGPS();
    setKalmanTsImu(0.01);
    setKalmanTsGps(0.5);
    initMatGlobal();

    initFilter(&filter_roll, 9 , coeffA_50Hz, coeffB_50Hz);
    initFilter(&filter_pitch, 9 , coeffA_50Hz, coeffB_50Hz);
    initFilter(&filter_yaw, 9 , coeffA_50Hz, coeffB_50Hz);


    initFilter(&filter_gx, 9 , coeffA_100Hz, coeffB_100Hz);
    initFilter(&filter_gy, 9 , coeffA_100Hz, coeffB_100Hz);
    initFilter(&filter_gz, 9 , coeffA_100Hz, coeffB_100Hz);

    initFilter(&filter_ax, 9 , coeffA_100Hz, coeffB_100Hz);
    initFilter(&filter_ay, 9 , coeffA_100Hz, coeffB_100Hz);
    initFilter(&filter_az, 9 , coeffA_100Hz, coeffB_100Hz);

    initFilter(&filter_gx2, 11 , coeffA_300Hz, coeffB_300Hz);
    initFilter(&filter_gy2, 11 , coeffA_300Hz, coeffB_300Hz);
    initFilter(&filter_gz2, 11 , coeffA_300Hz, coeffB_300Hz);


    initDNotchFilter(&dnotch_gx, 64, 50, 1000, 1, 10);
    initDNotchFilter(&dnotch_gy, 64, 50, 1000, 1, 10);
    initDNotchFilter(&dnotch_gz, 64, 50, 1000, 1, 10);

    initDNotchFilter(&dnotch_gx2, 64, 50, 1000, 1, 5);
    initDNotchFilter(&dnotch_gy2, 64, 50, 1000, 1, 5);
    initDNotchFilter(&dnotch_gz2, 64, 50, 1000, 1, 5);
    
    initDNotchFilter(&dnotch_ax, 64, 40, 1000, 1, 10);
    initDNotchFilter(&dnotch_ay, 64, 40, 1000, 1, 10);
    initDNotchFilter(&dnotch_az, 64, 40, 1000, 1, 10);

    myIMU.off_ax = 1808.2934570313;
    myIMU.off_ay = -4216.2314453125;
    myIMU.off_az = 417.3168334961;
    myIMU.scl_acc = 416.3453063965;

    myIMU.off_mx = -300.4071350098;
    myIMU.off_my = -161.3137817383;
    myIMU.off_mz = -196.4404296875;
    myIMU.scl_magx = 156.9006958008;
    myIMU.scl_magy = 162.3730010986;
    myIMU.scl_magz = 184.1072998047;

    calibrateGyro(&myIMU);
    //calibrateAccel(&myIMU);
    //calibrateMag(&myIMU);
    

    initTimer(&timer_accel, &accelInterrupt, 1000);
    initTimer(&timer_gyro, &gyroInterrupt, 1000);
    initTimer(&timer_mag, &magInterrupt, 10);
    initTimer(&timer_rpy, &rpyInterrupt, 500);
    initTimer(&timer_gps, &gpsInterrupt, 200);
    initTimer(&timer_pos, &positionInterrupt, 100);
}

void executeSensorsTasks(){
    
    if(timerReady(&timer_rpy))  executeTimer(&timer_rpy);
    if(timerReady(&timer_accel))  executeTimer(&timer_accel);
    if(timerReady(&timer_gyro))  executeTimer(&timer_gyro);  
    if(timerReady(&timer_mag))  executeTimer(&timer_mag);  
    if(timerReady(&timer_gps))  executeTimer(&timer_gps ); 
    if(timerReady(&timer_pos)) executeTimer(&timer_pos);
}