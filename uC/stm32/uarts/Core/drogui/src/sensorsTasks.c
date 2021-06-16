#include "sensorsTasks.h"
#include "mahony.h"
#include "task.h"
#include "utils.h"
#include "M8Q.h"
#include "kalman.h"
#include "serial.h"
#include "opticalFlow.h"
#include "ICM20948.h"
#include "MPU9250.h"
#include "teraRanger.h"
#include "macros.h"

imu myIMU;

mahony myRPY;

m8q myGPS;
optFlow myOF;
tRanger myTera;

filter filter_roll, filter_pitch, filter_yaw;

float   roll,       pitch,      yaw,
		raw_roll,   raw_pitch,  raw_yaw,
		ax,         ay,         az,
		gx,         gy,         gz,
		mx,         my,         mz,
		x,          y,          z,
		x_gps,		y_gps,
		xp,         yp,
		z_of,       z_tera;

bool mag_available = false;

float z_ant = 0;
float Kdfilt = 0.01;
bmp388 myBMP;

emaFilter ema_bmp;
mvAvgFilter mvAvg_bmp;
filter filter_z;
int cfilt_z = 0;

void accelTask(){

	readAcc(&myIMU);
	ax = -myIMU.ay, ay = myIMU.ax, az = myIMU.az;

	if( calib_status & 1 ){
		cleanFiltAcc(&myIMU.fAccX);
		cleanFiltAcc(&myIMU.fAccY);
		cleanFiltAcc(&myIMU.fAccZ);
		calib_status ^= 1;
	}

	setReg(ACC_X,(float)(ax));
	setReg(ACC_Y,(float)(ay));
	setReg(ACC_Z,(float)(az));
}

void gyroTask(){

	readGyro(&myIMU);
	gx = -myIMU.gy, gy = myIMU.gx, gz = myIMU.gz;

	if( calib_status & 2 ){

		cleanFiltGyro(&myIMU.fGyroX);
		cleanFiltGyro(&myIMU.fGyroY);
		cleanFiltGyro(&myIMU.fGyroZ);
		calib_status ^= 2;
	}

	setReg(GYRO_X,(float)(gx));
	setReg(GYRO_Y,(float)(gy));
	setReg(GYRO_Z,(float)(gz));

}

void magTask(){
	readMag(&myIMU);
	mx = -myIMU.mx;
	my = myIMU.my;
	mz = -myIMU.mz;
	setReg(MAG_X, mx);
	setReg(MAG_Y, my);
	setReg(MAG_Z, mz);
	mag_available = true;
}

void altitudeTask(){

	bmp388ReadAltitude(&myBMP);
	//z = computeMvAvgFilter( &mvAvg_bmp, myBMP.altitude );
	z = computeEmaFilter( &ema_bmp, myBMP.altitude);
	z = computeFilter( &filter_z, z );

	setReg(Z_VAL, z);

}


void gpsTask(){

	SENSOR_STATUS ret = readLatLon(&myGPS);
	setReg(GPS_STATE, ret);

	if(ret == OK){
		if(getReg(START_GPS) <= 0)
			myGPS.off_x = myGPS.latitude, myGPS.off_y = myGPS.longitud;

		int x_lat = myGPS.latitude - myGPS.off_x;
		int y_lon = myGPS.longitud - myGPS.off_y;
		x_gps = myGPS.latitude;
		y_gps = myGPS.longitud;

        setReg(GPS_AVAILABLE, 1);
        setReg(GPS_X, 0.01*x_lat),
        setReg(GPS_Y, 0.01*y_lon),
        setReg(GPS_CNT, myGPS.cnt++);     
    }
    else if( ret == CRASHED ){
        if(state == ARM_MOTORS || state == CONTROL_LOOP)
			state = DESCEND;
    }
}


void optTask(){

	OPT_VAR var;
	SENSOR_STATUS ret = readFlowRange(&myOF, &var);
	setReg(OPT_STATE, ret);

	if(ret == OK){
		if( z >= 0.05)
			xp  = -myOF.vel_x*0.001, yp = myOF.vel_y*0.001;
		else
			xp = yp = 0;
		if(myOF.dis != -1) z_of= myOF.dis*0.001;
		setReg(XP_VAL, xp), setReg(YP_VAL, yp), setReg(Z_RNG, z_of);

	}
	else if(ret == CRASHED){
		if(state == ARM_MOTORS || state == CONTROL_LOOP)
			state = DESCEND;
	}
	else if(ret == NO_DATA){
		xp = 0; yp = 0; z_of = 0;
	}

}

void teraTask(){

	SENSOR_STATUS ret = readTeraRange(&myTera);

	if(ret == OK)
		z_tera = myTera.distance/1000.0;
	else if(ret == CRASHED){
		if(state == ARM_MOTORS || state == CONTROL_LOOP)
			state = DESCEND;
	}
	else if(ret == NO_DATA)
		z_tera = 0;

}

void rpyTask(){

	float rpy[3];

	mahonyUpdate(&myRPY, gx*PI/180.0, gy*PI/180.0, gz*PI/180.0, ax, ay, az, mx, my, mz);
	getMahonyEuler(&myRPY, rpy);
	raw_roll = rpy[0], raw_pitch = rpy[1], raw_yaw = rpy[2]+pi/2;

	/*roll += fmax(fmin(Kdfilt, (rpy[0] - roll)),-Kdfilt);
	  pitch += fmax(fmin(Kdfilt, (rpy[1] - pitch)),-Kdfilt);
	  yaw += fmax(fmin(Kdfilt,(rpy[2] - yaw)),-Kdfilt);*/

	raw_roll = computeFilter(&filter_roll, raw_roll);
	raw_pitch = computeFilter(&filter_pitch, raw_pitch);
	raw_yaw = computeFilter(&filter_yaw, raw_yaw);

	setReg(RAW_ROLL, raw_roll);
	setReg(RAW_PITCH, raw_pitch);
	setReg(RAW_YAW, raw_yaw);

	roll = raw_roll - getReg(ROLL_OFFSET);
	pitch = raw_pitch - getReg(PITCH_OFFSET);
	yaw = raw_yaw - getReg(YAW_OFFSET);

	setReg(ROLL_VAL, roll);
	setReg(PITCH_VAL, pitch);
	setReg(YAW_VAL, yaw);

	if( calib_status & 8  ){
		cleanFilter(&filter_roll);
		cleanFilter(&filter_pitch);
		cleanFilter(&filter_yaw);
		calib_status ^= 8;
	}
}

void xyzTask(){


	if(getReg(START_GPS) > 0){
		kalmanUpdateIMU(ax, ay, az, raw_roll, raw_pitch, raw_yaw);

		if(getReg(GPS_AVAILABLE) > 0)
			setReg(GPS_AVAILABLE, 0),
				kalmanUpdateGPS(getReg(GPS_X), getReg(GPS_Y), 0);
	}
	else{
		clearKalman();
	}

	getPosition(&x, &y, &z);

	z = z_of;
	if(z_tera >= 0.5 && z_tera <= 50)
		z = z_tera;
	/*if( cfilt_z <= 50 && fabs(z-z_ant) > 0.2) cfilt_z++, z = z_ant;
	  else z_ant = z,  cfilt_z = 0;*/


	setReg(X_VAL, x);
	setReg(Y_VAL, y);
	setReg(Z_VAL, z);
}

void initSensorsTasks(){

	initImu(&myIMU);
	initMahony(&myRPY, 2, 0.1, 500);


	initFilter(&filter_roll, 6, k_1_10, v_1_10);
	initFilter(&filter_pitch, 6, k_1_10, v_1_10);
	initFilter(&filter_yaw, 6, k_1_10, v_1_10);

	setReg(ACC_SCALE,1);
	setReg(MAG_X_SCALE,1);
	setReg(MAG_Y_SCALE,1);
	setReg(MAG_Z_SCALE,1);

	setKalmanTsImu(0.01);
	setKalmanTsGps(0.125);

	initMatGlobal();

	initM8Q(&myGPS, SER_GPS);
	initOptFlow(&myOF, SER_OPT);
	initTeraRanger(&myTera, SER_TER);


	calib_status = 0;

	//initBmp388(&myBMP, 10);
	//initMvAvgFilter(&mvAvg_bmp, 25);

	initEmaFilter(&ema_bmp, 0.9, 0.1, 0.8);
	initFilter(&filter_z, 4, k_1_20, v_1_20);

	addTask(&gyroTask, 1000, 3);
	addTask(&accelTask, 1000, 3);
	addTask(&magTask, 100000, 2);
	addTask(&rpyTask, 2000, 2);
	//addTask(&altitudeTask,10000,2);
	addTask(&xyzTask, 10000, 3);
	addTask(&gpsTask, 125000, 3);
	addTask(&optTask, 10000, 1);
	addTask(&teraTask, 10000, 1);


}
