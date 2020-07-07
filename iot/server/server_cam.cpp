#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 

using namespace cv;
using namespace std;
void mat2Buff(Mat* im, unsigned char buff[]);
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 0; 
	int addrlen = sizeof(address); 
	unsigned char buffer[480*640] = {0}; 
	char * hello = "Hello from server"; 

	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) { 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 

	  Mat frame, outImg;
    VideoCapture cap;
    int deviceID = 1;
    int apiID = cv::CAP_ANY;
  int lazo;
    cap.open(deviceID + apiID);
    
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    
    cout << "Start grabbing" << "\n"<< "Press any key to terminate" << endl;
    for (;;){
        cap.read(frame);
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        
		cv::resize(frame, outImg, cv::Size(50,50), 0, 0, INTER_LINEAR);
        
		mat2Buff(&outImg, buffer);
        //sprintf(buffer,"Hola Inglis\n");
		int c = 0;
		while (c < 50*50){
	    	c += send(new_socket , buffer , 50*50 , 0 ); 
			printf("%d\n", c);
		}
        //sprintf("Buffer: %s\n",buffer);
        printf("Image sent\n");
        //cin>>lazo;
		sleep(1);
    }
    
    

	return 0; 
} 

  void mat2Buff(Mat* im,unsigned char buff[]){
  uchar* p;
  for (int i = 0; i < im->rows; ++i) {
      p = im->ptr<uchar>(i);
      for (int j = 0; j < im->cols; ++j) {
        buff[i * im -> cols + j] = p[j];
      }
  	}
  }