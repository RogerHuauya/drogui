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
#include "control.h"
#include "menu.h"
#include "threaded.h"
#include <pthread.h>

rasp_I2C rasp_i2c(DSPIC_ADDRESS);
#define POWERKEY 6

bool inputReceived = false, logging_state = false;
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
	sim7600.PowerOn(POWERKEY);
}





int main(int argc, char** argv ){
        enable_emergency_stop();
        srand((unsigned) time(NULL));
        setup();
        printf("Program has started\n");
        pthread_t threads[NUM_THREADS];
        //id_thread_log  = pthread_create(&threads[0], NULL, logging, (void *)0);
        id_thread_gps = pthread_create(&threads[2], NULL, gps_data, (void *)0);
        printf("Threads created \n");
        menu();
    return 0;
}
