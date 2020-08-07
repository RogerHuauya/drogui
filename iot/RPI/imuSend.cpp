#include "sockets.h"
#include "serial.h"

#define HEIGHT 480
#define WIDTH 640
#define BUFF_LENGTH (HEIGHT*WIDTH)

int main(int argc, char** argv ){
	
	int port = atoi(argv[1]);
	Socket drone = Socket("190.232.68.139", port);
	unsigned char buffer[BUFF_LENGTH]; 
	printf("Error: %d\n", drone.clientStart());
    
    char read_buf [256];
	memset(&read_buf, '\0', sizeof(read_buf));

    while(1){   
    }
    return 0;
} 