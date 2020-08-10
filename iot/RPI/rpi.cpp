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

int main(int argc, char** argv ){
	
	int port = atoi(argv[1]);
	Socket drone = Socket("192.168.1.19", port);
	unsigned char buffer[BUFF_LENGTH];

	int cstart = drone.clientStart();
	if(cstart) printf("Error %d\n", cstart);

	/*cv::Mat image(HEIGHT, WIDTH, CV_8UC1);
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);*/
    std::string msg;
    while(1){   
		drone.readJson(&msg);
		cout<<"Message received :"<< msg;
		msg.clear();
		sleep(2);
    }
    return 0;
} 