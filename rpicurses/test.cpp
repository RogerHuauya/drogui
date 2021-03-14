#define raspberry
#define BUFF_LENGTH 500
#define DSPIC_ADDRESS 0x60
#include <iostream>
#include <ctime>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <unistd.h> 
#include <fstream>
#include <string.h>
#include <vector>

std::vector<float> parsestring( std::string s, std::string delit  ){

	int pos_start = 0, pos_end, delim_len = delit.length();
    std::string token;
    std::vector<float> parse_s;

    while ((pos_end = s.find (delit, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        parse_s.push_back(std::stof(token));
    }
    
    
    parse_s.push_back(std::stof(s.substr(pos_start)));
    return parse_s;
}

int main(int argc, char** argv ){

	std::fstream offsetfile;
	std::string offset="";
	std::vector<float> offset_angles;

	offsetfile.open("../rpicurses/memory/offset_angles.txt",std::ios::in);

	if (offsetfile.is_open()){
		getline(offsetfile, offset);  
      	offsetfile.close();
	}  

	offset_angles = parsestring(offset, " ");

    for( int i = 0; i < offset_angles.size(); i++ ) std::cout << offset_angles[i] << " ";


    return 0;
}
