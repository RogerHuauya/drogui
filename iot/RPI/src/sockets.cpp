#include "sockets.h"
typedef unsigned char uchar;
int Socket::Connect(){
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){ 
        perror("\nConnection Failed \n"); 
        return -1; 
    } 
    return 0;
}

int Socket::Create(){
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
        perror("\n Socket creation error \n"); 
        return -1; 
    }
    return 0;
}

int Socket::Address(){
    if(inet_pton(AF_INET,ip, &addr.sin_addr)<=0){ 
        perror("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
    return 0;
}

int Socket::Attach(){
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){ 
		perror("setsockopt"); 
		return -1;
	} 
    return 0;
}

int Socket::Bind(){
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr))<0) { 
		perror("bind failed"); 
		return -1;
	} 
    return 0;
}

int Socket::Listen(){
    if (listen(sock, 3) < 0) { 
		perror("listen"); 
		return -1; 
	} 
    return 0;
}

int Socket::Accept(){
	if ((child_socket = accept(sock, (struct sockaddr *)&addr, (socklen_t *)&addrlen))<0) { 
		perror("accept "); 
        return -1;
	} 
    return 0;
}

int Socket::clientStart(){
    if(Create() < 0)    return -1;
    if(Address() < 0)   return -2;
    if(Connect() <0)    return -3;
    return 0;
}

int Socket::serverStart(){
    if(Create() < 0)    return -1;
    if(Attach() < 0)    return -2;
    if(Bind() < 0)      return -3;
    if(Listen() < 0)    return -4;
    if(Accept() < 0)    return -5;
    return 0;
}

void Socket::readBuffer(unsigned char buff[], int len){
    valread = 0;
    while(valread < len){
        valread += read(sock , buff + valread, len - valread);
    } 
}

void Socket::sendBuffer(unsigned char buff[], int len){
    valread = 0;
    while(valread < len){
        valread += send(child_socket , buff + valread, len - valread, 0);
    } 
}

void Socket::sendJson(std::string s){
    uint dataLenght = s.size();
    uchar p[50];
    for(int i = 0; i < sizeof(uint); i++) 
        p[i] = (dataLenght >> (8*i)) & (0xFF);

    send(child_socket, p, sizeof(uint), MSG_CONFIRM);
    send(child_socket, s.c_str(), dataLenght, MSG_CONFIRM);
}

void Socket::readJson(std::string * s){
    uchar num[50];
    char buffer[5000];
    uint valread = 0, len = 0;
    read(sock, num, sizeof(uint));
    for(int i = 0; i < sizeof(uint); i++) {
        len = len | (num[i] << (8*i));
    }
    while(valread < len){
        valread += read(sock, buffer + valread, len - valread);
    } 
    buffer[len] = '\0';
    *s = buffer;
}