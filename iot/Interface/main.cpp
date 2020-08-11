#include <bits/stdc++.h>
#include "sockets.h"
#include "camera.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <jsoncpp/json/json.h>
#include <time.h>
#include <cstdlib>
using namespace std;

#define red(n)      "\033[1;31m"#n"\033[0m"
#define green(n)    "\033[1;32m"#n"\033[0m"
#define yellow(n)   "\033[1;33m"#n"\033[0m"
#define blue(n)     "\033[1;34m"#n"\033[0m"
#define white(n)     "\033[1;37m"#n"\033[0m"

#define HEIGHT 480
#define WIDTH 640

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
string s;
char buff[500];

Socket base;

void cls(){
    system("clear");
}

void startServer(){
    cls();
    printf("Starting server ...\n");
    int err = base.serverStart();
    if (err!= 0) printf("The error %d has occurred, please verify network\n", err);
    else printf("Incoming connection from drone has been established\n \
                waiting for instructions ...\n");
    //string msg = "Hello from Roger Server\n";
    s = fw.write(root);
    base.sendJson(s);

}


void emergencyStop(){
    s = fw.write(root);
    cout << s << endl;
    base.sendJson(s);
}

void desplazamiento(){
    
    double dx, dy, dz, dphi;
    Json::Value desplazamiento(Json::arrayValue);
    cls();
    printf(white(Insertar desplazamiento en metros y grados sexagesimales) "\n" blue((dx, dy, dz, dphi))"\n");
    cin >> dx >> dy >> dz >> dphi;
    desplazamiento.append(dx);
    desplazamiento.append(dy);
    desplazamiento.append(dz);
    desplazamiento.append(dphi);
    root["desplazamiento"] = desplazamiento;
    s = fw.write(root);
    cout << s << endl;
    base.sendJson(s);
}

void dataSensor(){
    s = fw.write(root);
    cout << s << endl;
    base.sendJson(s);
    base.readJson(&s);
    cout<<"Data recieved: "<<s<<endl;
}

void showImage(){

    s = fw.write(root);
    int camera;
    cls();
    printf(white(Insertar camara\n) green([1]) " " white(ELP\n) green([2])" " white(Makerfocus\n));
    cin >> camera;
    root["camera"] = camera;
    s = fw.write(root);
    cout << s << endl;
    base.sendJson(s);
    base.readJson(&s);
    Json::Reader r;
    Json::Value v;
    r.parse(s, v);
	cv::Mat image(HEIGHT, WIDTH,  CV_8UC1);
    buff2Mat(&image, (unsigned char *)v['image'].asCString());
    cv::namedWindow("Image from drone", cv::WINDOW_AUTOSIZE);
    while(1){
        cv::imshow("Image from drone",image);
        cv::waitKey(0);
    }
}


void finalCoordinates(){
    
    double x, y, z, phi;
    Json::Value position(Json::arrayValue);

    cls();
    printf(white(Insertar posición final en \n metros y grados sexagesimales) "\n" blue((dx, dy, dz, dphi)) "\n");
    cin >> x >> y >> z >> phi;

    position.append(x);
    position.append(y);
    position.append(z);
    position.append(phi);
    root["position"] = position;
    s = fw.write(root);
    cout << s << endl;
    base.sendJson(s);
}

void ARM(){
    s = fw.write(root);
    cout << s << endl;
    base.sendJson(s);
}

void calibrateESC(){
    s = fw.write(root);
    cout << s << endl;
    base.sendJson(s);
}


void zeroPosition(){
    s = fw.write(root);
    cout << s << endl;
    base.sendJson(s);
}



int menu(){
    cls();    
    root.clear();
    printf("\t\t\t\t" blue(Principal menu) "\n");
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
    printf("\n");
    printf(blue(Please enter an option >>>));
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
    sleep(2);
    return 0;
}



int main(int argc, char const *argv[]) { 
	

    int port = atoi(argv[1]);
	cout<<"Port elected: "<<port<<endl;
	base = Socket(INADDR_ANY, port);
   
    while(1){
        menu();
    }
    return 0;
}
