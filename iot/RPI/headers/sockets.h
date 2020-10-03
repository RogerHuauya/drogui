#ifndef SOCKETS_H
#define SOCKETS_H
#include <string>
#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <stdio.h>
#include <unistd.h> 
struct Socket{
    char* ip;
    int port, sock, valread, child_socket, addrlen, opt;
    sockaddr_in addr;
    
    int Create();
    int Address();
    int Connect();
    
    int Attach();
    int Bind();
    int Listen();
    int Accept();

    int clientStart();
    int serverStart();

    void readBuffer(unsigned char buff[], int len);
    void sendBuffer(unsigned char buff[], int len);



    Socket(const char* _ip, int _port){
        ip = (char *)_ip, port = _port;
        addr.sin_family = AF_INET; 
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;  
        addrlen = sizeof(addr);
        opt = 0;
    }

    Socket(){};
    void sendJson(std::string s);
    void readJson(std::string * s);
};


#endif