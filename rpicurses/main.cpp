#define raspberry
#define BUFF_LENGTH 500
#define DSPIC_ADDRESS 0x60
#include <iostream>
#include "registerMap.h"
#include "arduPi.h"
#include "sim7600.h"
#include <ctime>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <unistd.h> 

#include "utils.h"
#include "curmenu.h"
#include "threaded.h"
#include <pthread.h>

rasp_I2C rasp_i2c(DSPIC_ADDRESS);
#define POWERKEY 6

bool inputReceived = false;
bool cin_thread = false;
int id_choosen, value; 
int id_thread_menu, id_thread_log, id_thread_gps;

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
	sim7600.PowerOn(POWERKEY);
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


	//printf("Program has started\n");
	pthread_t threads[NUM_THREADS];
	id_thread_log  = pthread_create(&threads[0], NULL, logging, (void *)0);
	//id_thread_gps = pthread_create(&threads[2], NULL, gps_data, (void *)0);
	//printf("Threads created \n");
	curmenu();

    return 0;
}
