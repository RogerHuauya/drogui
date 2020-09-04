#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>
#include "camera.h"
#include "sockets.h"

using namespace std;
using namespace Json;

#define HEIGHT 480
#define WIDTH 640
#define BUFF_LENGTH (HEIGHT*WIDTH)

Socket drone;
Reader r;
FastWriter fw;
Value root;
string s;
Camera cam;
uchar buff[5000000];


void desplazamiento(){
	auto v = root["desplazamiento"];
	printf("X: %lf, Y: %lf, Z: %lf, alpha: %lf\n", v[0].asDouble(), \
	v[1].asDouble(), v[2].asDouble(), v[3].asDouble());
}

void dataSensor(){
	root.clear();
	root["imu"] = 5.42;
	root["pressure"] = 1033.05;
	root["current"] = 0.03;
	s = fw.write(root);
	drone.sendJson(s);
}

void showImage(){
	root.clear();
	cam.getFrame();
	cv::Size siz = cam.bwframe.size(); 
	cout << siz.height << " " << siz.width << endl;
	mat2Buff(&cam.bwframe, buff);
	string s2((char*)buff);
	root["image"] = s2;
	cout << s2.size() << endl;
	s = fw.write(root);

	cout << s.size() << endl;
	drone.sendJson(s);
}

void finalCoordinates(){
	auto v = root["position"];
	printf("X: %lf, Y: %lf, Z: %lf, alpha: %lf\n", v[0].asDouble(), \
	v[1].asDouble(), v[2].asDouble(), v[3].asDouble());
}

int main(int argc, char** argv ){
	
	int port = atoi(argv[1]);
	drone = Socket("190.236.166.65", port);
	unsigned char buffer[BUFF_LENGTH];

	int cstart = drone.clientStart();
	if(cstart) printf("Error %d\n", cstart);
	

	cam = Camera(0);
    cam.open();
	
	std::string msg;
    
	while(1){

		drone.readJson(&msg);
		r.parse(msg, root);

		int func = root["function"].asInt();

		switch(func){
			case 1: printf("Connected\n"); 		break;
			case 2: printf("Emergency\n"); 		break;
			case 3: desplazamiento();			break;
			case 4: dataSensor(); 				break;
			case 5: showImage();				break;
			case 6: finalCoordinates();			break;
			case 7: printf("ARM\n");			break;
			case 8: printf("Calibrate ESC\n");	break;
			case 9: printf("Zero position\n");	break;
			default: break;
		}

		msg.clear();
		sleep(2);
    }
    return 0;
}
