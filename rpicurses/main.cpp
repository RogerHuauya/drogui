#define raspberry
#define BUFF_LENGTH 500
#define DSPIC_ADDRESS 0x60
#include <iostream>
#include "registerMap.h"
//#include "arduPi.h"
#include "sim7600.h"
#include <ctime>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <unistd.h> 
#include <fstream>

#include "utils.h"
#include "curmenu.h"
#include "threaded.h"
#include <pthread.h>
#include <string.h>
#include <vector>

rasp_I2C rasp_i2c(DSPIC_ADDRESS);

#define POWERKEY 6

bool inputReceived = false;
bool cin_thread = false;
int id_choosen, value; 
int id_thread_wifi, id_thread_log, id_thread_gps;

void enable_emergency_stop(){
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handler_stop;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    return;
}

void setup() {
	rasp_i2c.setup();
	//sim7600.PowerOn(POWERKEY);
	std::fstream fil;
	float aux[6];

	fil.open("../rpicurses/memory/offset_angles.txt",std::ios::in);
	if (fil.is_open()){
		fil >> aux[0] >> aux[1] >> aux[2]; 
      	fil.close();
	}
	rasp_i2c.sendFloat(ROLL_OFFSET, aux[0]);
	rasp_i2c.sendFloat(PITCH_OFFSET,aux[1]);
	rasp_i2c.sendFloat(YAW_OFFSET, 	aux[2]);

	fil.open("../rpicurses/memory/cal_gyr.txt",std::ios::in);
	if (fil.is_open()){
		fil >> aux[0] >> aux[1] >> aux[2]; 
      	fil.close();
	}
	rasp_i2c.sendFloat(GYR_X_OFF, aux[0]);
	rasp_i2c.sendFloat(GYR_Y_OFF, aux[1]);
	rasp_i2c.sendFloat(GYR_Z_OFF, aux[2]);

	fil.open("../rpicurses/memory/cal_acc.txt",std::ios::in);
	if (fil.is_open()){
		fil >> aux[0] >> aux[1] >> aux[2] >> aux[3]; 
      	fil.close();
	}
	rasp_i2c.sendFloat(ACC_X_OFF, aux[0]);
	rasp_i2c.sendFloat(ACC_Y_OFF, aux[1]);
	rasp_i2c.sendFloat(ACC_Z_OFF, aux[2]);
	rasp_i2c.sendFloat(ACC_SCALE, aux[3]);
	
	fil.open("../rpicurses/memory/cal_mag.txt",std::ios::in);
	if (fil.is_open()){
		fil >> aux[0] >> aux[1] >> aux[2] >> aux[3] >> aux[4] >> aux[5]; 
      	fil.close();
	}
	rasp_i2c.sendFloat(MAG_X_OFF, 	aux[0]);
	rasp_i2c.sendFloat(MAG_Y_OFF, 	aux[1]);
	rasp_i2c.sendFloat(MAG_Z_OFF, 	aux[2]);
	rasp_i2c.sendFloat(MAG_X_SCALE, aux[3]);
	rasp_i2c.sendFloat(MAG_Y_SCALE, aux[4]);
	rasp_i2c.sendFloat(MAG_Z_SCALE, aux[5]);
}

void reset(){
    rasp_i2c.sendFloat(X_REF, 0);
    rasp_i2c.sendFloat(Y_REF, 0);
    rasp_i2c.sendFloat(Z_REF, 0);
    rasp_i2c.sendFloat(ROLL_REF, 0);
    rasp_i2c.sendFloat(PITCH_REF, 0);
    rasp_i2c.sendFloat(YAW_REF, 0);
}

int main(int argc, char** argv ){
		
	enable_emergency_stop();
	srand((unsigned) time(NULL));
	setup();
	reset();


	pthread_t threads[3];
	id_thread_log  = pthread_create(&threads[0], NULL, logging, (void *)0);
	//id_thread_gps = pthread_create(&threads[2], NULL, gps_data, (void *)0);
	id_thread_wifi = pthread_create(&threads[1], NULL, wifiCheck, (void *)0);
	curmenu();

	return 0;
}
