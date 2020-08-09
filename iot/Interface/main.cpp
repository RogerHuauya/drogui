#include <bits/stdc++.h>
#include "sockets.h"
#include <jsoncpp/json/json.h>
#include <time.h>
#include <cstdlib>
#include <sstream>
#include <zlib.h>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "camera.h"

std::string compress_string(const std::string& str,
                            int compressionlevel = Z_BEST_COMPRESSION)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();           // set the z_stream's input

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}


using namespace std;

unsigned char buffer [500000];
#define red(n)      "\033[1;31m"#n"\033[0m"
#define green(n)    "\033[1;32m"#n"\033[0m"
#define yellow(n)   "\033[1;33m"#n"\033[0m"
#define blue(n)     "\033[1;34m"#n"\033[0m"
#define white(n)     "\033[1;37m"#n"\033[0m"


/*
         foreground background
black        30         40
red          31         41
green        32         42
yellow       33         43
blue         34         44
magenta      35         45
cyan         36         46
white        37         47
*/


Json::Value root;
Json:: FastWriter fw;
string s, s2;
char buff[500];
Camera c;

Socket base;

void cls(){
    system("clear");
}

void startServer(){
    int err = base.serverStart();
    if (err!= 0) printf("The error %d has occurred, please verify network\n", err);
    else printf("Server initialized successfully");
}


void emergencyStop(){
    s = fw.write(root);
    s.pop_back();
    cout << s << " size: " << s.size() <<endl;
    s2 = compress_string(s);
    cout << s2 << "  size: "<< s2.size()<<endl;
    //base.sendJson(s);
}

void desplazamiento(){
    
    double dx, dy, dz, dphi;
    Json::Value desplazamiento(Json::arrayValue);
    
    cls();
    printf(white(Insertar desplazamiento en metros y sexagesimales) "\n" blue((dx, dy, dz, dphi))"\n");
    cin >> dx >> dy >> dz >> dphi;
    desplazamiento.append(dx);
    desplazamiento.append(dy);
    desplazamiento.append(dz);
    desplazamiento.append(dphi);
    root["desplazamiento"] = desplazamiento;
    s = fw.write(root);
    s.pop_back();
    cout << s << " size: " << s.size() <<endl;
    s2 = compress_string(s);
    cout << s2 << "  size: "<< s2.size()<<endl;
    //base.sendJson(s);
}

void dataSensor(){
    s = fw.write(root);
    s.pop_back();
    cout << s << " size: " << s.size() <<endl;
    s2 = compress_string(s);
    cout << s2 << "  size: "<< s2.size()<<endl;
    //base.sendJson(s);
}

void showImage(){

    s = fw.write(root);
    int camera;
    cls();
    printf(white(Insertar camara\n) green([1]) " " white(ELP\n) green([2])" " white(Makerfocus\n));
    cin >> camera;
    root["camera"] = camera;
    c.getFrame();
    mat2Buff(&c.bwframe, buffer);
    string s3((char*) buffer);
    root["fame"] = s3;
    s = fw.write(root);
    s.pop_back();
    cout << " size: " << s.size() <<endl;
    s2 = compress_string(s);
    cout << "  size: "<< s2.size()<<endl;
    //base.sendJson(s);
    
    cv::imshow("Display Image", c.bwframe);
    cv::waitKey(0);
}


void finalCoordinates(){
    
    double x, y, z, phi;
    Json::Value position(Json::arrayValue);

    cls();
    printf(white(Insertar posición final en metros y grados sexagesimales) "\n" blue((dx, dy, dz, dphi)) "\n");
    cin >> x >> y >> z >> phi;

    position.append(x);
    position.append(y);
    position.append(z);
    position.append(phi);
    root["position"] = position;
    s = fw.write(root);
    s.pop_back();
    cout << s << " size: " << s.size() <<endl;
    s2 = compress_string(s);
    cout << s2 << "  size: "<< s2.size()<<endl;
    //base.sendJson(s);
}

void ARM(){
    s = fw.write(root);
    s.pop_back();
    cout << s << " size: " << s.size() <<endl;
    s2 = compress_string(s);
    cout << s2 << "  size: "<< s2.size()<<endl;
    //base.sendJson(s);
}

void calibrateESC(){
    s = fw.write(root);
    s.pop_back();
    cout << s << " size: " << s.size() <<endl;
    s2 = compress_string(s);
    cout << s2 << "  size: "<< s2.size()<<endl;
    //base.sendJson(s);
}


void zeroPosition(){
    s = fw.write(root);
    s.pop_back();
    cout << s << " size: " << s.size() <<endl;
    s2 = compress_string(s);
    cout << s2 << "  size: "<< s2.size()<<endl;
    //base.sendJson(s);
}



int menu(){
std::system("clear");
    root.clear();
    printf("\t\t\t\t\t\t\t\t" blue(Principal menu) "\n");
    printf(green([1]) " " white( Start server\n));
    printf(green([2]) " " white( Emergency stop\n));
    printf(green([3]) " " white(Desplazamiento\n));
    printf(green([4]) " " white(Show data sensor\n));
    printf(green([5]) " " white(Show image\n));
    printf(green([6]) " " white(Final coordinates\n));
    printf(green([7]) " " white(ARM\n));
    printf(green([8]) " " white(Calibrar ESC\n));
    printf(green([9]) " " white(Zero position\n));
    int op;
    cin>>op;
    root["function"] = op;
    switch(op){
        case 1: startServer(); break;
        case 2: emergencyStop(); break;
        case 3: desplazamiento(); break;
        case 4: dataSensor(); break;
        case 5: showImage(); break;
        case 6: finalCoordinates(); break;
        case 7: ARM(); break;
        case 8: calibrateESC(); break;
        case 9: zeroPosition(); break;
        default: printf("%d is not an option, please enter option again\n", op); menu(); break;
    }
    
    sleep(3);
    return 0;
}



int main(int argc, char const *argv[]) { 
	c = Camera(0);
    c.open();
    /*
    int port = atoi(argv[1]);
	cout<<"Port elected: "<<port<<endl;
	Socket base = Socket(" ", port);
    */
   
    while(1){
        menu();
    }
    return 0;
}
