#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
using namespace cv;


void buffToMat(Mat* im, char buff[]){
    uchar* p;
    for (int i = 0; i < im->rows; ++i) {
        p = im->ptr<uchar>(i);
        for (int j = 0; j < im->cols; ++j) {
            buff[i * im -> cols + j] = p[j];
        }
    }
}

int main(int argc, char** argv )
{
    int sock = 0, valread; 
	struct sockaddr_in serv_addr;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(8888); 

	if(inet_pton(AF_INET, "190.232.68.139", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	
    

	char buffer[480*640 + 10] = {0}; 
    Mat image(640, 480, CV_8UC1);

    namedWindow("Display Image", WINDOW_AUTOSIZE );
    
    while(1){   
        printf("Hola\n");
        valread = read( sock , buffer, 480*640 + 10); 
        //buffToMat(&image, buffer);
        printf("%s\n", buffer);
        //imshow("Display Image", image);
    }
    return 0;

}