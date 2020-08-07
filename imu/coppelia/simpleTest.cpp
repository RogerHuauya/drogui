#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>
#include <sys/ioctl.h>
#include <iostream>
using namespace std;

int serial_port;
struct termios tty;

void initSerial();
bool kbhit();

extern "C" {
    #include "extApi.h"
}

int main(int argc,char* argv[]){

    initSerial();
    
    int clientID = simxStart((simxChar*)"190.232.68.139",9001,true,true,2000,5);
    if (clientID!=-1){

        printf("Connected to remote API server\n");
    }
    else return 0;
    
    char read_buf [200];
    char word[40];
    float quaternion[4];
    int pos = 0, posq = 0;
    int n;
	memset(&read_buf, '\0', sizeof(read_buf));


	while(1){

		memset(&read_buf, '\0', sizeof(read_buf));
		int num_bytes = read(serial_port, read_buf, sizeof(read_buf));
		

        for(int i = 0 ; read_buf[i] != '\0'; i++ ){
            word[pos++] = read_buf[i];
            if(read_buf[i] == ','){
                pos = 0;
                quaternion[posq++] = stof(word);
                memset(&word, '\0', sizeof(word));
            }
            if(read_buf[i] == '\n'){
                quaternion[posq++] = stof(word);
                //for(int i = 0; i< 4; i++) cout << quaternion[i] << " ";  cout << endl; 
                int result=simxCallScriptFunction(clientID,"remoteApiCommandServer",sim_scripttype_childscript,\
                                                        "displayText_function",0,NULL,4,quaternion,0,NULL,0,NULL,\
                                                        NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,simx_opmode_blocking);
                
                memset(&word, '\0', sizeof(word));
                pos = posq = 0;
            }
        }
        usleep(100);
    }
    printf("hola\n");

	close(serial_port);
    //simxFinish(clientID);
    return(0);
}



void initSerial(){
    serial_port = open("/dev/ttyACM0", O_RDWR);
	memset(&tty, 0, sizeof tty);

	if(tcgetattr(serial_port, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}
	tty.c_cflag &= ~PARENB; 
	tty.c_cflag &= ~CSTOPB; 
	tty.c_cflag |= CS8; // 8 bits per byte (most common)
	tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR;
	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);


	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}
}
