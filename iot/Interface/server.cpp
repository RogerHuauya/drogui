#include <bits/stdc++.h>
#include "camera.h"
#include "sockets.h"
#define HEIGHT 480
#define WIDTH 640
#define BUFF_LENGTH (HEIGHT*WIDTH)

int main(int argc, char const *argv[]) { 
	int port = atoi(argv[1]);
	std::cout<<"Port elected: "<<port<<std::endl;
	unsigned char buffer[BUFF_LENGTH]; 
	Socket base = Socket(" ", port);
	Camera my_cam = Camera(0);
	std::cout<<base.serverStart()<<"\n";
	my_cam.open();
    while(1){
        my_cam.getFrameAsBuffer(buffer);
		base.sendBuffer(buffer, BUFF_LENGTH);
		usleep(10000);
    }
	return 0; 
} 