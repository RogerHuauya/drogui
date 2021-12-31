#include "sensorsTasks.h"
#include "mahony.h"
#include "task.h"
#include "utils.h"
#include "kalman.h"
#include "gps.h"
#include "serial.h"
#include "opticalFlow.h"
#include "imu.h"
#include "rangeFinder.h"
#include "flysky.h"
#include "macros.h"

imu myIMU;

mahony myRPY;

gps myGPS;
kalman myKalman;
optFlow myOF;
rangeFinder myRange;
fsReceiver myFS;

filter filter_roll, filter_pitch, filter_yaw;

float   roll,       pitch,      yaw,
		raw_roll,   raw_pitch,  raw_yaw,
		roll_fs,     pitch_fs,    yaw_fs,   h_fs,
		ax,         ay,         az,
		gx,         gy,         gz,
		mx,         my,         mz,
		x,          y,          z,
		x_gps,		y_gps,
		vx_gps,		vy_gps,
		xp,         yp,
		z_of,       z_rng,
		vx,			vy;


float z_ant = 0;
float Kdfilt = 0.01;
bmp388 myBMP;

emaFilter ema_bmp;
mvAvgFilter mvAvg_bmp;
filter filter_z;
int cfilt_z = 0;


void accelTask(){

	readAcc(&myIMU);
	ax = myIMU.ax, ay = myIMU.ay, az = myIMU.az;

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
	gx = myIMU.gx, gy = myIMU.gy, gz = myIMU.gz;

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
	mx = myIMU.mx;
	my = myIMU.my;
	mz = myIMU.mz;
	setReg(MAG_X, mx);
	setReg(MAG_Y, my);
	setReg(MAG_Z, mz);
}

void altitudeTask(){
	bmp388ReadAltitude(&myBMP);
	z = computeEmaFilter( &ema_bmp, myBMP.altitude);
	z = computeFilter( &filter_z, z );
	setReg(Z_VAL, z);
}

void flyskyTask(){

	SENSOR_STATUS ret = readFsReceiver(&myFS);

	if(ret == OK){
		
		roll_fs  = myFS.channel_val[0];
		pitch_fs = myFS.channel_val[1];
		h_fs   = myFS.channel_val[2];
		yaw_fs     = myFS.channel_val[3];

	}
	else if( ret == CRASHED ){
		serialPrint(SER_DBG, "FLYSKY Crashed\n");
		if(state == ARM_MOTORS || state == CONTROL_LOOP)
			state = DESCEND;
	}
}

void gpsTask(){

	SENSOR_STATUS ret = readGPS(&myGPS);
	setReg(GPS_STATE, ret);

	if(ret == OK){
		if(getReg(START_GPS) <= 0)
			myGPS.off_x = myGPS.longitud, myGPS.off_y = myGPS.latitude;

		int x_lon = myGPS.longitud - myGPS.off_x;
		int y_lat = myGPS.latitude - myGPS.off_y;

		vx_gps = myGPS.east_vel/1000.0;
		vy_gps = myGPS.north_vel/1000.0;

		x_gps = myGPS.longitud;
		y_gps = myGPS.latitude;

		setReg(GPS_AVAILABLE, 1);
		setReg(GPS_X, 0.01*x_lon),
		setReg(GPS_Y, 0.01*y_lat),
		setReg(GPS_VX, vx_gps),
		setReg(GPS_VY, vy_gps);
		//setReg(GPS_CNT, myGPS.cnt++);
	}
		else if( ret == CRASHED ){
		serialPrint(SER_DBG, "GPS Crashed\n");
		if(state == ARM_MOTORS || state == CONTROL_LOOP)
			state = DESCEND;
	}
}


void optTask(){

	OPT_VAR var;
	SENSOR_STATUS ret = readFlowRange(&myOF, &var);

	if(ret == OK){
		if(myOF.dis != -1) z_of= myOF.dis*0.001;
		//setReg(XP_VAL, xp), setReg(YP_VAL, yp);
		setReg(Z_RNG, z_of);
	}
	else if(ret == CRASHED){
		serialPrint(SER_DBG, "OPT Crashed\n");
		if(state == ARM_MOTORS || state == CONTROL_LOOP)
			state = DESCEND;
	}
}

void rangeTask(){

	SENSOR_STATUS ret = readRangeFinder(&myRange);

	if(ret == OK)
		z_rng = (myRange.distance - myRange.offset)/1000.0;
	else if(ret == CRASHED){
		serialPrint(SER_DBG, "Range Crashed\n");
		if(state == ARM_MOTORS || state == CONTROL_LOOP)
			state = DESCEND;
	}
}

void rpyTask(){

	float rpy[3];

	mahonyUpdate(&myRPY, gx*PI/180.0, gy*PI/180.0, gz*PI/180.0, ax, ay, az, mx, my, mz);
	getMahonyEuler(&myRPY, rpy);
	raw_roll = rpy[0], raw_pitch = rpy[1], raw_yaw = rpy[2] + pi/2;

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

	z = z_rng;

	if(getReg(START_GPS) > 0){
		kalmanUpdateIMU(&myKalman, ax, ay, az, raw_roll, raw_pitch, raw_yaw);

		if(getReg(GPS_AVAILABLE) > 0){
			setReg(GPS_AVAILABLE, 0);
			#ifdef KALMANP
			kalmanUpdateGPS(&myKalman, getReg(GPS_X), getReg(GPS_Y), z);
			#elif defined KALMANPV
			kalmanUpdateGPS(&myKalman, getReg(GPS_X), getReg(GPS_Y), getReg(GPS_VX), getReg(GPS_VY));
			#endif
		}
	}
	else{
		clearKalman(&myKalman);
	}

	#ifdef KALMANP
	getPosition(&myKalman, &x, &y, &z);
	#elif defined KALMANPV
	getPosition(&myKalman, &x, &y);
	#endif

	z = z_rng;

	getVelocity(&myKalman, &vx, &vy);

	xp = vx*cos(raw_yaw) + vy*sin(raw_yaw);
	yp = -vx*sin(raw_yaw) + vy*cos(raw_yaw);

	setReg(X_VAL, x);
	setReg(Y_VAL, y);
	setReg(Z_VAL, z);

	setReg(XP_VAL, vx);
	setReg(YP_VAL, vy);
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

	setKalmanTsImu(&myKalman, 0.01);
	setKalmanTsGps(&myKalman, 0.125);

	initMatGlobal(&myKalman);

	initGPS(&myGPS, SER_GPS);
	//initOptFlow(&myOF, SER_OPT);
	initRangeFinder(&myRange, SER_RNG);
	initFsReceiver(&myFS, SER_FSK);
	//setReg(CAL_FS_TRG,1);
	
	calib_status = 0;

	//initBmp388(&myBMP, 10);
	//initMvAvgFilter(&mvAvg_bmp, 25);

	initEmaFilter(&ema_bmp, 0.9, 0.1, 0.8);
	initFilter(&filter_z, 4, k_1_20, v_1_20);

	addTask(&gyroTask, 1000, 3);
	addTask(&accelTask, 1000, 3);
	addTask(&magTask, 20000, 2);
	addTask(&rpyTask, 2000, 2);
	//addTask(&altitudeTask,10000,2);
	addTask(&xyzTask, 10000, 3);
	addTask(&gpsTask, 125000, 3);
	//addTask(&optTask, 10000, 1);
	addTask(&rangeTask, 10000, 1);
	addTask(&flyskyTask,10000, 1);
}
