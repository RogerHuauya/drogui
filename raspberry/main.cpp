#define raspberry
#define BUFF_LENGTH 500
#define DSPIC_ADDRESS 0x60
#define pi 3.141592
#include <iostream>
#include "registerMap.h"
#include "arduPi.h"
#include "sim7600.h"
#include <ctime>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#ifdef raspberry
#include "utils.h"
#include "control.h"
#include <pthread.h>
rasp_I2C rasp_i2c(DSPIC_ADDRESS);
#endif
#define POWERKEY 6

bool inputReceived = false, logging_state = false;
bool cin_thread = false;
int id_choosen, value; 
int id_thread_menu, id_thread_log, id_thread_gps;

std::ofstream log_file;

void setup() {
	sim7600.PowerOn(POWERKEY);
}

void sleep(unsigned milliseconds){
    unistd::usleep(milliseconds * 1000); // takes microseconds
}


void *logging(void *threadid){
    
    while(1){
        //ofstream log_file;
        //std::string name_log = str_datetime(); 
        while(!logging_state){}
        std::string name_log = str_datetime(); 
        log_file.open("logs/"+name_log+".txt");
        /*
        log_file << "H_VAL   H_STEP_SIZE " << rasp_i2c.readFloat(H_VAL) << " " << rasp_i2c.readFloat(H_STEP_SIZE)<< std::endl;
        log_file << "ROLL KP KI KD " << rasp_i2c.readFloat(ROLL_KP) << " " <<rasp_i2c.readFloat(ROLL_KI) << " " <<rasp_i2c.readFloat(ROLL_KD)<<std::endl;
        log_file << "PITCH KP KI KD " << rasp_i2c.readFloat(PITCH_KP) << " " <<rasp_i2c.readFloat(PITCH_KI) << " " <<rasp_i2c.readFloat(PITCH_KD)<<std::endl;
        log_file << "YAW KP KI KD " << rasp_i2c.readFloat(YAW_KP) << " " <<rasp_i2c.readFloat(YAW_KI) << " " <<rasp_i2c.readFloat(YAW_KD)<<std::endl;
        */
        while(1){
            /*
            log_file<<rasp_i2c.readFloat(H_VAL);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(ROLL_DEG)*180.0/pi+180;
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(PITCH_DEG)*180.0/pi+180;
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(YAW_DEG)*180.0/pi+180;
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(ROLL_REF);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(PITCH_REF);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(YAW_REF);
	    log_file<<"\t";
	    log_file<<rasp_i2c.readFloat(PRESS_ABS) << std::endl;
        */
       /*
            log_file<<rasp_i2c.readFloat(RAW_TEMP);
            log_file<<"\t";           
            log_file<<rasp_i2c.readFloat(TEMP_ABS);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(RAW_PRESS);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(PRESS_ABS);
            */
            log_file<<rasp_i2c.readFloat(Z_REF);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(Z_U);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(Z_VAL);
            log_file<<"\t";
            log_file<<std::endl;
            //unistd::usleep(50000); // takes microseconds
            sleep(100);
            if(!logging_state) break;
        }
    }
}

void *gps_data(void *threadid){
    sim7600.GPSStart();
    float offset_x = 0, offset_y = 0, r = 1;
    for(int i = 0; i < 10; i++){
        if(sim7600.GPSGet()){
            offset_x += r*sim7600.Log*cos(sim7600.Lat*pi/180);
            offset_y += r*sim7600.Lat;
        }
        offset_x /= 10;
        offset_y /= 10;
    }
    while(1){
        if(sim7600.GPSGet()){
            rasp_i2c.sendFloat(GPS_X, r*sim7600.Log*cos(sim7600.Lat*pi/180) - offset_x);
            rasp_i2c.sendFloat(GPS_Y, r*sim7600.Lat - offset_y);
        }
        delay(100);
    }
}
int main(int argc, char** argv ){
        enable_emergency_stop();
        srand((unsigned) time(NULL));
        setup();
        printf("Program has started\n");
        pthread_t threads[NUM_THREADS];
        id_thread_log  = pthread_create(&threads[0], NULL, logging, (void *)0);
        //id_thread_gps = pthread_create(&threads[2], NULL, gps_data, (void *)0);
        printf("Threads created \n");


        while(1){
            if(!cin_thread){
                std::cin.clear();
                printf("id function : \n");
                std::cin>>id_choosen;
                if(std::cin.fail()) throw 505;
                printf("function choosen: \n");
                cin_thread=true;
                inputReceived = true;
            }
        }

    return 0;
}
