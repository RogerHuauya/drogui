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
    else printf("Socket created ...\n");
    return 0;
}

int Socket::Address(){
    if(inet_pton(AF_INET,ip, &addr.sin_addr)<=0){ 
        perror("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }
    else printf("Address is correct");
    return 0;
}

int Socket::Attach(){
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){ 
		perror("setsockopt"); 
		return -1;
	}
    printf("Socket attached ...\n"); 
    return 0;
}

int Socket::Bind(){
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr))<0) { 
		perror("bind failed"); 
		return -1;
	} 
    printf("Binding terminated ...\n");
    return 0;
}

int Socket::Listen(){
    if (listen(sock, 3) < 0) { 
		perror("listen"); 
		return -1; 
	} 
    printf("Socket is listening ...\n");
    return 0;
}

int Socket::Accept(){
    printf("Socket is waiting for incoming connection ...\n");
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
    child_socket = sock;
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
        valread += read(child_socket , buff + valread, len - valread);
    } 
}

void Socket::sendBuffer(unsigned char buff[], int len){
    valread = 0;
    while(valread < len){
        valread += send(child_socket , buff + valread, len - valread, 0);
    } 
}

void Socket::sendJson(std::string s){
    unsigned int dataLenght = s.size();
    unsigned char p[50];
    for(int i = 0; i < sizeof(unsigned int); i++) p[i] = (dataLenght >> (8*i)) & (0xFF);
    std::cout<<"Lenght sent "<< dataLenght<<std::endl;
    send(child_socket, p, sizeof(unsigned int), MSG_CONFIRM);
    send(child_socket, s.c_str(), dataLenght, MSG_CONFIRM);
}

void Socket::readJson(std::string * s){
    uchar num[50];
    char buffer[500000];
    uint valread = 0;
    uint len = 0;
    read(child_socket, num, sizeof(uint));
    for(int i = 0; i < sizeof(uint); i++) {
        std::cout<<num[i]<<" "<<int(num[i])<<'\n';
        len = len | (num[i] << (8*i));
    }
    std::cout<<"len data: "<<len<<'\n';
    while(valread < len){
        valread += read(child_socket, buffer + valread, len - valread);
    } 
    *s = buffer;
}