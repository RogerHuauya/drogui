#include "camera.h"
#include "sockets.h"

#define HEIGHT 480
#define WIDTH 640
#define BUFF_LENGTH (HEIGHT*WIDTH)

int main(int argc, char** argv ){
	
	int port = atoi(argv[1]);
	Socket drone = Socket("190.232.68.139", port);
	unsigned char buffer[BUFF_LENGTH]; 
	printf("Error: %d\n", drone.clientStart());

	cv::Mat image(HEIGHT, WIDTH, CV_8UC1);
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    
    while(1){   
		drone.readBuffer(buffer, BUFF_LENGTH);
		buff2Mat(&image, buffer);
		cv::imshow("Display Image", image);
		cv::waitKey(1);
    }
    return 0;
} 