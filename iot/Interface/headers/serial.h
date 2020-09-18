#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h> 
#include <unistd.h> 

struct Serial{
    int serial_port;
    struct termios tty;

    Serial(const char* COM){
        serial_port =  open(COM, O_RDWR);
    }

    void initSerial();
    void readBuffer(char* read_buf, int sz);

};

#endif