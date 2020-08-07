#include <bits/stdc++.h>
#include "sockets.h"
#include <jsoncpp/json/json.h>
#include <time.h>
#include <cstdlib>
using namespace std;

Json::Value root;
Json:: FastWriter fw;
string s;
char buff[500];

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
    cout << s << endl;
    base.sendJson(s);
}

void desplazamiento(){
    
    double dx, dy, dz, dphi;
    Json::Value desplazamiento(Json::arrayValue);
    
    cls();
    printf("Insertar desplazamiento en metros y sexagesimales (dx, dy, dz, dphi)\n");
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
}

void showImage(){

    s = fw.write(root);
    int camera;
    cls();
    printf("Insertar camara\n[1] ELP\n[2] Makerfocus\n");
    cin >> camera;
    root["camera"] = camera;
    s = fw.write(root);
    cout << s << endl;
    base.sendJson(s);
}


void finalCoordinates(){
    
    double x, y, z, phi;
    Json::Value position(Json::arrayValue);

    cls();
    printf("Insertar posición final en metros y grados sexagesimales (dx, dy, dz, dphi)\n");
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
std::system("clear");
    root.clear();
    printf("\t\t Principal menu\n");
    printf("[1] Start server\n");
    printf("[2] Emergency stop\n");
    printf("[3] Desplazamiento (dx, dy , dz, dphi)"\n);
    printf("[4] Show data sensor\n");
    printf("[5] Show image\n");
    printf("[6] Final coordinates\n");
    printf("[7] ARM\n");
    printf("[8] Calibrar ESC\n");
    printf("[9] Zero position\n");
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
    
    sleep(2);
}



int main(int argc, char const *argv[]) { 
	
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