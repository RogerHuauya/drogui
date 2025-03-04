#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
using namespace cv;
#define HEIGHT 480
#define WIDTH 640
#define BUFF_LENGTH (HEIGHT*WIDTH)
#define ip  "190.232.68.139"
#define port 8889 

void buffToMat(Mat* im, unsigned char buff[]){
	uchar* p;
	for (int i = 0; i < im->rows; ++i) {
		p = im->ptr<uchar>(i);
		for (int j = 0; j < im->cols; ++j) {
			p[j] = buff[i * im -> cols + j];
        }
    }
}


int main(int argc, char** argv ){

    int sock = 0, valread; 
	struct sockaddr_in serv_addr;
	unsigned char buffer[BUFF_LENGTH] = {0}; 
    Mat image(HEIGHT, WIDTH, CV_8UC1);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(port); 

	if(inet_pton(AF_INET,ip, &serv_addr.sin_addr)<=0){ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    
	namedWindow( "Display window", WINDOW_AUTOSIZE );
    while(1){   
		printf("Waiting for message\n");
		valread = 0;
		while(valread < BUFF_LENGTH){
        	valread += read(sock , buffer + valread, BUFF_LENGTH - valread);
		} 
		buffToMat(&image, buffer);
		imshow("Display Image", image);
		waitKey(1);
    }
    return 0;
} 