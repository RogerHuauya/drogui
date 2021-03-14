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
extern rasp_I2C rasp_i2c;

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
}

void reset(){
    rasp_i2c.sendFloat(X_REF, 0);
    rasp_i2c.sendFloat(Y_REF, 0);
    rasp_i2c.sendFloat(Z_REF, 0);
    rasp_i2c.sendFloat(ROLL_REF, 0);
    rasp_i2c.sendFloat(PITCH_REF, 0);
    rasp_i2c.sendFloat(YAW_REF, 0);
}

std::vector<float> parsestring( std::string s, std::string delit  ){

	int pos_start = 0, pos_end, delim_len = delit.length();
    std::string token;
    std::vector<float> parse_s;

    while ((pos_end = s.find (delit, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        parse_s.push_back(std::stof(token));
    }
    
    
    parse_s.push_back(std::stof(s.substr(pos_start)));
    return parse_s;
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
	id_thread_wifi = pthread_create(&threads[1], NULL, wifiCheck, (void *)0);
	//printf("Threads created \n");
	curmenu();
	
	std::fstream offsetfile;
	std::string offset="";
	std::vector<float> offset_angles;
	float offs_roll,offs_pitch,offs_yaw;

	offsetfile.open("../rpicurses/memory/offset_angles.txt",std::ios::in);

	if (offsetfile.is_open()){
		cin >> offs_roll >> offs_pitch >> offs_yaw;
      	offsetfile.close();
	}  

	offset_angles = parsestring(offset, " ");

	/*rasp_i2c.sendFloat(ROLL_OFFSET, offset_angles[0]);
	rasp_i2c.sendFloat(PITCH_OFFSET, offset_angles[1]);
	rasp_i2c.sendFloat(YAW_OFFSET, offset_angles[2]);*/

	cout << offs_roll << " " << offs_pitch <<" "<<offs_yaw;
	

	/*
	setup();

	uint8_t n, c[50], x;
	float ans = 5;
	std::string s;*/
	/*std::cout << "write[w], read[r]\n";
	while(std::cin >> n){
		switch(n){
			case 'w':
				std::cin >> s;
				write(rasp_i2c.file_id, s.c_str(), s.size());
			break;
			case 'r':
				read(rasp_i2c.file_id, &c, 2);
				c[2] = '\0';
				printf("%s\n",c);
			break;
		}
		std::cout << "write[w], read[r]\n";
	}*/
	/*for(int i = 0 ; i < 100 ; i++){
		ans = rasp_i2c.readFloat(N_FILTER);
		std::cout<<ans<<std::endl;
		usleep(1000);
	}*/
    return 0;
}
