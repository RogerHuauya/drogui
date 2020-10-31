#define raspberry

#include <iostream>
#include <fstream>
#include <unistd.h> 

#ifdef raspberry
#include <wiringPiI2C.h>
#include <pthread.h>
#endif

#include "registerMap.h"
#include "utils.h"
#include <ctime>
#include <cstdlib>


using namespace std;

#define BUFF_LENGTH 500
#define DSPIC_ADDRESS 0x60
int fd;
bool inputReceived = false;
int index_, value; 
int rc;
void cls(){
    system("clear");
}
uint8_t readMCU(uint8_t rogerperry);

/*
#include <iostream>

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
    /*
	//auto v = root["desplazamiento"];
	double x, y, z, alp;
	cin>>x>>y>>z>>alp;
	printf("X: %lf, Y: %lf, Z: %lf, alpha: %lf\n", x, y, z, alp);
	//printf("X: %lf, Y: %lf, Z: %lf, alpha: %lf\n", v[0].asDouble(), v[1].asDouble(), v[2].asDouble(), v[3].asDouble());
    */
}

void dataSensor(){
    while(true){
        uint8_t r = rand()%15, p = rand()%15, y = rand()%15;
        #ifdef raspberry
            //r = readMCU(Rroll);
            //p = readMCU(Rpitch);
            //y = readMCU(Ryaw);
        #endif
        //cls();
        //usleep(200);
        //printf(green(roll\t)  " "  green(pitch\t) " " green(yaw\t\n));
        printf("%d\t%d\t%d\n", r, p, y);
        //printf("prueba \n");
        if(inputReceived) break;
        //sleep(1);
        
    }
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

void zeroPosition(){

}
void calibrateESC(){}
void emergencyStop(){
	
}

void writeMCU(uint8_t reg, uint8_t val){
    #ifdef raspberry	
	wiringPiI2CWriteReg8 (fd, reg, val);
    #endif
    //cout<<"Register "<<reg<<" has been written with "<<val<<endl;
}
uint8_t readMCU(uint8_t reg){
    uint8_t val;
    #ifdef raspberry
	val = wiringPiI2CReadReg8 (fd, reg);
    #endif
    //cout<<"Register "<<reg<<" has been written with "<<val<<endl;
    return val;
}
void writeRegister(){
    while(!inputReceived){}
    inputReceived = false;
    writeMCU(index_, value);
}

void readRegister(){
    while(!inputReceived){}
    inputReceived = false;
    value = readMCU(index_);
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
    printf(white(Enter an option = \n));
     while(!inputReceived){
        // paralelizando
        //cout<<" roger "<<endl;
    };
    inputReceived = false;
    cout<<"menu "<<index_<<endl;
    sleep(1);
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
    //sleep(2);
    //return 0;
    }
    pthread_exit(NULL);
    return 0;
}
void sendFloat(uint8_t reg, float val){
    uint8_t buff[4];
    floattobytes(val, buff);
    print4bytes(buff);
    for(uint8_t i = 0; i < 4; i++) writeMCU(reg+i, buff[i]);
    return;
}
float readFloat(uint8_t reg){
    uint8_t buff[4];
    for (uint8_t i=0;i<4;i++)  buff[i] = readMCU(reg+i);
    return bytestofloat(buff);
}

int main(int argc, char** argv ){
    pthread_t threads[NUM_THREADS];
    srand((unsigned) time(NULL));

    #ifdef raspberry
    fd = wiringPiI2CSetup(DSPIC_ADDRESS);
    #endif
    cout<<"Program has started"<<endl;
    //rc = pthread_create(&threads[0], NULL, menu, (void *)0);
    cout<<"Thread created "<<endl;
    while(1){
        float value;
        cin>>value;
        sendFloat(M1_VAL, value);
        cout<<"Value sent : "<<endl;
        cout<<"Value confirm : "<<readFloat(M1_VAL)<<endl;
    }
    /*
    while(1){
        std::cin.clear();
        cin>>index_>>value;
        cout<<"main : "<<index_<<endl;
        inputReceived = true;
        writeRegister();
        inputReceived = true;
        readRegister();
        cout<<value<<endl;
    }*/
    /*
    while(1){
        menu();
        sleep(2);
    }*/
    return 0;
}
