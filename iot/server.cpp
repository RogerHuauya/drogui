#include <bits/stdc++.h>
#include "camera.h"
#include "sockets.h"
#define PORT 8080 
#define HEIGHT 480
#define WIDTH 640
#define BUFF_LENGTH (HEIGHT*WIDTH)

int main(int argc, char const *argv[]) { 
	int port = atoi(argv[0]);
	unsigned char buffer[BUFF_LENGTH]; 
	Socket base = Socket(" ", port);
	Camera my_cam = Camera(0);
	base.serverStart();
	my_cam.open();
    while(1){
        my_cam.getFrameAsBuffer(buffer);
		base.sendBuffer(buffer, BUFF_LENGTH);
		usleep(10000);
    }
	return 0; 
} 