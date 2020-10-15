#include <iostream>
#include <fstream>
#include <unistd.h> 
#include <wiringPiI2C.h>
#include <pthread.h>
#include "registerMap.h"
#include "utils.h"
using namespace std;

#define BUFF_LENGTH 500
#define DSPIC_ADDRESS 0x60
int fd;
bool inputReceived = false;
int index_, value; 
int rc;

/*
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 5

void *pLeerSerial(void *threadid) {
   long tid;
   tid = (long)threadid;
   cout << "Hello World! Thread ID, " << tid << endl;
   pthread_exit(NULL);
}

int main () {
   pthread_t threads[NUM_THREADS];
   int rc;
   int i;
   
   for( i = 0; i < NUM_THREADS; i++ ) {
      cout << "main() : creating thread, " << i << endl;
      rc = pthread_create(&threads[i], NULL, pLeerSerial, (void *)i);
      
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
   }
   pthread_exit(NULL);
}

gcc test.cpp -lpthread

*/
void desplazamiento(){
	//auto v = root["desplazamiento"];
	double x, y, z, alp;
	cin>>x>>y>>z>>alp;
	printf("X: %lf, Y: %lf, Z: %lf, alpha: %lf\n", x, y, z, alp);
	//printf("X: %lf, Y: %lf, Z: %lf, alpha: %lf\n", v[0].asDouble(), v[1].asDouble(), v[2].asDouble(), v[3].asDouble());

}

void dataSensor(){

	/*root.clear();
	root["imu"] = 5.42;
	root["pressure"] = 1033.05;
	root["current"] = 0.03;
	s = fw.write(root);
	drone.sendJson(s);
	*/
}

void finalCoordinates(){
	/*
	auto v = root["position"];
	printf("X: %lf, Y: %lf, Z: %lf, alpha: %lf\n", v[0].asDouble(), \
	v[1].asDouble(), v[2].asDouble(), v[3].asDouble());
	*/
}

void ARM(){

}
void cls(){
    system("clear");
}
void zeroPosition(){

}
void calibrateESC(){}
void emergencyStop(){
	
}

void writeMCU(uint8_t reg, uint8_t val){	
	wiringPiI2CWriteReg8 (fd, reg, val);
}
uint8_t readMCU(uint8_t reg){
	return wiringPiI2CReadReg8 (fd, reg);
}
void writeRegister(){
    while(!inputReceived){}
    inputReceived = false;
    writeMCU(index_, value);
}

void readRegister(){
    while(!inputReceived){}
    inputReceived = false;
    uint8_t val = readMCU(index_);
}

void *menu(void *threadid){
    while(1){
    cls();    
    printf("\t\t\t\t\t\t\t\t" blue(Principal menu) "\n");
    printf(green([0]) " " white(Emergency stop\n));
    printf(green([1]) " " white(Desplazamiento\n));
    printf(green([2]) " " white(Show data sensor\n));
    printf(green([3]) " " white(Final coordinates \n));
    printf(green([4]) " " white(ARM\n));
    printf(green([5]) " " white(Calibrar ESC \n));
    printf(green([6]) " " white(Zero position \n));
    printf(green([7]) " " white(Write register \n));
    printf(green([8]) " " white(Read register \n));
    printf("\n");
    printf(blue(Please enter an option >>>));
    while(!inputReceived){};
    inputReceived = false;
    switch(index_){
	case 0: emergencyStop(); break;
        case 1: desplazamiento(); break;
        case 2: dataSensor(); break;
        case 3: finalCoordinates(); break;
        case 4: ARM(); break;
        case 5: calibrateESC(); break;
        case 6: zeroPosition(); break;
        case 7: writeRegister(); break;
        case 8: readRegister(); break;
        default: printf("%d is not an option, please enter option again\n", index_); break;
    }
    sleep(2);
    //return 0;
    }
    pthread_exit(NULL);
    return 0;
}

int main(int argc, char** argv ){
    pthread_t threads[NUM_THREADS];
<<<<<<< HEAD
    fd = wiringPiI2CSetup (0x60);
    cout<<"Program has started"<<endl;
=======
 	fd = wiringPiI2CSetup(DSPIC_ADDRESS);
	cout<<"Program has started"<<endl;
>>>>>>> 6d7a5db6c10b510e9db830a8c7924a426c43fb80
    rc = pthread_create(&threads[0], NULL, menu, (void *)0);
    while(1){
        std::cin.clear();
        cin>>index_>>value;
        inputReceived = true;
        sleep(1);
    }
/*
while(1){
    menu();
    sleep(2);
}*/
    return 0;
}
