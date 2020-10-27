#ifndef SOCKETS_H
#define SOCKETS_H
#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <string>
#include <iostream>


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

<<<<<<< HEAD
    void sendJson(string s);
    void readJson(string s);
=======
    void sendJson(std::string s);
    void readJson(std::string * s);
>>>>>>> 73f29d6b904f8ab1a13527ba1a88c293307797fd

    Socket(const char* _ip, int _port){
        ip = (char *)_ip, port = _port;
        addr.sin_family = AF_INET; 
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;  
        addrlen = sizeof(addr);
        opt = 0;
    }

    Socket(){};
};


#endif