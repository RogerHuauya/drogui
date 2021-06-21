#include <bits/stdc++.h>
#include "sockets.h"
#include "utils.h"

using namespace std;

unsigned char buffer [500000];
#define red(n)      "\033[1;31m"#n"\033[0m"
#define green(n)    "\033[1;32m"#n"\033[0m"
#define yellow(n)   "\033[1;33m"#n"\033[0m"
#define blue(n)     "\033[1;34m"#n"\033[0m"
#define white(n)    "\033[1;37m"#n"\033[0m"

string s, s2;
char buff[500];
Socket serverRoger;

void cls(){
    system("clear");
}

int main(int argc, char const *argv[]) {
    int port = atoi(argv[1]);
	cout << "Port elected: "<< port <<endl;
    serverRoger.serverStart();
    return 0;
}
