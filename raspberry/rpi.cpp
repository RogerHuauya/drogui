#include <iostream>
#include <fstream>
#include <unistd.h> 
#include "bcm2835.h"
#include "arduPi.h"

WirePi dspic;

using namespace std;

#define red(n)      "\033[1;31m"#n"\033[0m"
#define green(n)    "\033[1;32m"#n"\033[0m"
#define yellow(n)   "\033[1;33m"#n"\033[0m"
#define blue(n)     "\033[1;34m"#n"\033[0m"
#define white(n)    "\033[1;37m"#n"\033[0m"
#define BUFF_LENGTH 500

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
void emergencyStop(){}

void sendMCU(uint8_t reg, uint8_t val){}
void sendMCU(uint8_t reg, uint8_t* vals, int n){}
uint8_t readMCU(uint8_t reg){}
void readMCU(uint8_t reg, uint8_t* vals){}


int menu(){
    cls();    
    printf("\t\t\t\t\t\t\t\t" blue(Principal menu) "\n");
    printf(green([0]) " " white( Emergency stop\n));
    printf(green([1]) " " white(Desplazamiento\n));
    printf(green([2]) " " white(Show data sensor\n));
    printf(green([3]) " " white(Final coordinates\n));
    printf(green([4]) " " white(ARM\n));
    printf(green([5]) " " white(Calibrar ESC\n));
    printf(green([6]) " " white(Zero position\n));
    int op;
    printf("\n");
    printf(blue(Please enter an option >>>));
    cin>>op;
    switch(op){
		case 0: emergencyStop(); break;
        case 1: desplazamiento(); break;
        case 2: dataSensor(); break;
        case 3: finalCoordinates(); break;
        case 4: ARM(); break;
        case 5: calibrateESC(); break;
        case 6: zeroPosition(); break;
        default: printf("%d is not an option, please enter option again\n", op); menu(); break;
    }
    sleep(2);
    return 0;
}

int main(int argc, char** argv ){
	dspic.begin();
	dspic.beginTransmission(0x20);
	dspic.requestFrom(0x10, 1);
	dspic.endTransmission();
	char ans = dspic.read();
	cout<<"answer is: "<<ans<<endl;
	/*
	while(1){
		menu();
		sleep(2);
    }*/
    return 0;
}
