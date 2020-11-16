#define raspberry
#define BUFF_LENGTH 500
#define DSPIC_ADDRESS 0x60
#define pi 3.141592
#include <iostream>
#include "registerMap.h"
#include "arduPi.h"
#include "sim7x00.h"
#include <ctime>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#ifdef raspberry
#include "utils.h"
#include <pthread.h>
rasp_I2C rasp_i2c(DSPIC_ADDRESS);
#endif
#define POWERKEY 6
using namespace std;


bool inputReceived = false, logging_state = false;
bool cin_thread = false;
int id_choosen, value; 
int id_threads;
int id_threads_log;

ofstream log_file;

void setup() {
	sim7600.PowerOn(POWERKEY);
}

void sleep(unsigned milliseconds)
{
    unistd::usleep(milliseconds * 1000); // takes microseconds
}

void desplazamiento(){
    /*
	//auto v = root["desplazamiento"];
	double x, y, z, alp;
	cin>>x>>y>>z>>alp;
	printf("X: %lf, Y: %lf, Z: %lf, alpha: %lf\n", x, y, z, alp);
	//printf("X: %lf, Y: %lf, Z: %lf, alpha: %lf\n", v[0].asDouble(), v[1].asDouble(), v[2].asDouble(), v[3].asDouble());
    */
}

void dataSensor(){
    int cnt= 0;
    while(true){
        float r = rand()%15, p = rand()%15, y = rand()%15;
        #ifdef raspberry
            r = rasp_i2c.readFloat(ROLL_DEG)*180.0/pi+180;
            p = rasp_i2c.readFloat(PITCH_DEG)*180.0/pi+180;
            y = rasp_i2c.readFloat(YAW_DEG)*180.0/pi+180;
        #endif
        //cls();
        //usleep(50000);s
        //printf(green(roll\t)  " "  green(pitch\t) " " green(yaw\t\n));
        printf("%d\t %f\t%f\t%f\n",cnt, r, p, y);
        //printf("prueba \n");
        cnt++;
        if(inputReceived) break;
        //sleep(1);        
    }
	/*root.clear();
	root["imu"] = 5.42;
	root["pressure"] = 1033.05;
	root["current"] = 0.03;
	s = fw.write(root);
	drone.sendJson(s);
	*/

}

void zeroPosition(){
    return;

}
void normalStop(){
	return;
}
void handler_stop(int s){
    normalStop();
    printf("Emergency exit CTRL+C - Caught signal %d\n",s);
    exit(1); 
}
void enable_emergency_stop(){
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handler_stop;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    return;
}

void send_PID_ROLL(){    
    cin_thread=true;
    cls(); 
    float value1,value2,value3;
    printf(green(PID ROLL) "\n");
    cout<<"Set index:"<<endl;
    cin>>value1;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(PID_INDEX, value1);
    cout<<"KP KI KD = "<<endl;
    cin>>value1>>value2>>value3;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(ROLL_KP, value1);
    rasp_i2c.sendFloat(ROLL_KI, value2);
    rasp_i2c.sendFloat(ROLL_KD, value3);
    cout<<"Values sent : "<<endl;
    sleep(1);
    cin_thread=false;
    return;
}
void send_PID_PITCH(){    
    cin_thread=true;
    cls(); 
    float value1,value2,value3;
    printf(green(PID PITCH) "\n");
    cout<<"Set index:"<<endl;
    cin>>value1;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(PID_INDEX, value1);
    cout<<"KP KI KD = "<<endl;
    cin>>value1>>value2>>value3;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(PITCH_KP, value1);
    rasp_i2c.sendFloat(PITCH_KI, value2);
    rasp_i2c.sendFloat(PITCH_KD, value3);
    cout<<"Values sent : "<<endl;
    sleep(1);
    cin_thread=false;
    return;
}

void send_H(){
    cin_thread=true;
    cls(); 
    float value1,value2;
    printf(green(H value and stepsize) "\n");
    cout<<"H deltaH = "<<endl;
    cin>>value1>>value2;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(H_VAL, value1);
    rasp_i2c.sendFloat(H_STEP_SIZE, value2);
    cout<<"Values sent : "<<endl;
    if(value1 == 0) logging_state = false;
    else logging_state = true;
    sleep(1);
    cin_thread=false;
    return; 
}
void send_PID_YAW(){    
    cin_thread=true;
    cls(); 
    float value1,value2,value3;
    printf(green(PID YAW) "\n");
    cout<<"Set index:"<<endl;
    cin>>value1;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(PID_INDEX, value1);
    cout<<"KP KI KD = "<<endl;
    cin>>value1>>value2>>value3;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(YAW_KP, value1);
    rasp_i2c.sendFloat(YAW_KI, value2);
    rasp_i2c.sendFloat(YAW_KD, value3);
    cout<<"Values sent : "<<endl;
    sleep(1);
    cin_thread=false;
    return;
}

void send_setpoint(){
    cin_thread=true;
    int op; 
    float val;
    cls();
    printf("\t\t\t\t\t\t\t\t" blue(Setpoint menu) "\n");
    printf(green([0]) " " white(SP_ROLL\n));
    printf(green([1]) " " white(SP_PITCH\n));
    printf(green([2]) " " white(SP_YAW\n));
    cin >> op;
    cout<<"Value :" << endl;
    cin>> val;
    if(cin.fail()) throw 505;
    switch (op)
    {
    case 0:
        rasp_i2c.sendFloat(ROLL_REF, val);
        break;
    case 1:
        rasp_i2c.sendFloat(PITCH_REF, val);
        break;
    case 2:
        rasp_i2c.sendFloat(YAW_REF, val);
        break;

    }
    cin_thread=false;
    return;
}

void writeRegister(){
    cin_thread=true;
    cls(); 
    int reg;
    float value;
    cout<<"REG  FLOAT = "<<endl;
    cin>>reg>>value;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat((uint8_t)reg, value);
    cout<<"Value sent : "<<value<<endl;
    cout<<"Value confirm : "<<rasp_i2c.readFloat(reg)<<endl;
    sleep(1);
    cin_thread=false;
    return;
}
void readRegister(){
    cin_thread=true;
    cls(); 
    int reg;
    
    printf("\t\t\t\t\t\t\t\t" blue(Principal menu) "\n");
    printf(green([0]) " " white(H_dH\n));
    printf(green([1]) " " white(PID_ROLL\n));
    printf(green([2]) " " white(PID_PITCH\n));
    printf(green([3]) " " white(PID_YAW\n));
    printf(green([4]) " " white(SETPOINTS\n));

    cin >> reg;
    if(cin.fail()) throw 505;
    switch(reg){
        case 0: cout << rasp_i2c.readFloat(H_VAL) << " " << rasp_i2c.readFloat(H_STEP_SIZE) << endl; break;
        case 1: cout << rasp_i2c.readFloat(ROLL_KP) << " ";
                 cout << rasp_i2c.readFloat(ROLL_KI) << " ";
                 cout << rasp_i2c.readFloat(ROLL_KD) << endl; break;
        
        case 2: cout << rasp_i2c.readFloat(PITCH_KP) << " ";
                 cout << rasp_i2c.readFloat(PITCH_KI) << " ";
                 cout << rasp_i2c.readFloat(PITCH_KD) << endl; break;
        
        case 3:cout << rasp_i2c.readFloat(YAW_KP) << " ";
                 cout << rasp_i2c.readFloat(YAW_KI) << " ";
                 cout << rasp_i2c.readFloat(YAW_KD) << endl; break;

        case 4:cout << rasp_i2c.readFloat(ROLL_REF) << " ";
                 cout << rasp_i2c.readFloat(PITCH_REF) << " ";
                 cout << rasp_i2c.readFloat(YAW_REF) << endl; break;
    }

    sleep(3000);
    cin_thread=false;
    return;
}
void send_TS(){
    cin_thread=true;
    cls(); 
    float value1;
    printf(green(Tsampling) "\n");
    cout<<"Ts = "<<endl;
    cin>>value1;
    if(cin.fail()) throw 505;
    rasp_i2c.sendFloat(TS_CONTROL, value1);
    cout<<"Values sent : "<<endl;
    sleep(1);
    cin_thread=false;
    return; 
}
void send_AT_command(){
    cin_thread = true;
    cls();
    char at_command[100];
    memset(at_command, '\0', 100);    // Initialize the string
    delay(100);
    while (Serial.available() > 0) Serial.read();    // Clean the input buffer
    while(1){
        printf("Please input the AT command: \n>>>");
	    scanf("%s", at_command);
        if(at_command[0] == '0') break;
        Serial.println(at_command);
        sim7600.sendATcommand(at_command, 2000);
    }

    cin_thread = false;
    return;
}
void getGPSdata(){
    cls();
    sim7600.GPSPositioning();
    delay(5000);
    return;
}
void *logging(void *threadid){
    
    while(1){
        //ofstream log_file;
        //std::string name_log = str_datetime(); 
        while(!logging_state){}
        //log_file.open("logs/"+name_log);

        log_file << "H_VAL   H_STEP_SIZE " << rasp_i2c.readFloat(H_VAL) << " " << rasp_i2c.readFloat(H_STEP_SIZE)<< endl;
        log_file << "ROLL KP KI KD " << rasp_i2c.readFloat(ROLL_KP) << " " <<rasp_i2c.readFloat(ROLL_KI) << " " <<rasp_i2c.readFloat(ROLL_KD)<<endl;
        log_file << "PITCH KP KI KD " << rasp_i2c.readFloat(PITCH_KP) << " " <<rasp_i2c.readFloat(PITCH_KI) << " " <<rasp_i2c.readFloat(PITCH_KD)<<endl;
        log_file << "YAW KP KI KD " << rasp_i2c.readFloat(YAW_KP) << " " <<rasp_i2c.readFloat(YAW_KI) << " " <<rasp_i2c.readFloat(YAW_KD)<<endl;
        while(1){
            log_file<<rasp_i2c.readFloat(H_VAL);
            log_file<<" ";
            log_file<<rasp_i2c.readFloat(YAW_DEG)*180.0/pi+180;
            log_file<<" ";
            log_file<<rasp_i2c.readFloat(ROLL_DEG)*180.0/pi+180;
            log_file<<" ";
            log_file<<rasp_i2c.readFloat(PITCH_DEG)*180.0/pi+180;
            log_file<<" ";
            log_file<<rasp_i2c.readFloat(ROLL_REF);
            log_file<<" ";
            log_file<<rasp_i2c.readFloat(PITCH_REF);
            log_file<<" ";
            log_file<<rasp_i2c.readFloat(YAW_REF)<<endl;
            //unistd::usleep(50000); // takes microseconds
            sleep(100);
            if(!logging_state) break;
        }
    }
}
void *menu(void *threadid){
    while(1){
        cls();    
        printf("\t\t\t\t\t\t\t\t" blue(Principal menu) "\n");
        printf(green([0]) " " white(Emergency stop\n));
        printf(green([1]) " " white(Desplazamiento\n));
        printf(green([2]) " " white(Show data sensor\n));
        printf(green([3]) " " white(Send PID ROLL \n));
        printf(green([4]) " " white(Send PID PITCH\n));
        printf(green([5]) " " white(Send PID YAW  \n));
        printf(green([6]) " " white(Zero position \n));
        printf(green([7]) " " white(Send H \n));
        printf(green([8]) " " white(Sample period (ms) PID TS \n));
        printf(green([9]) " " white(Write register \n));
        printf(green([10]) " " white(Read register \n));
        printf(green([11]) " " white(Send AT command \n));
        printf(green([12]) " " white(GPS position \n));
        printf(green([13]) " " white(Send setpoint \n));
        printf(white(Enter an option = \n));
        while(!inputReceived){
            // paralelizando
            //cout<<" roger "<<endl;
        };
        inputReceived = false;
        cout<<"menu : "<<id_choosen<<endl;
        //sleep(1);
        switch(id_choosen){
            case 0: normalStop(); break;
            case 1: desplazamiento(); break;
            case 2: dataSensor(); break;
            case 3: send_PID_ROLL(); break;
            case 4: send_PID_PITCH(); break;
            case 5: send_PID_YAW(); break;
            case 6: zeroPosition(); break;
            case 7: send_H(); break;
            case 8: send_TS(); break;
            case 9: writeRegister(); break;
            case 10: readRegister(); break;
            case 11: send_AT_command(); break;
            case 12: getGPSdata(); break;
            case 13: send_setpoint(); break;
            default: printf("%d is not an option, please enter option again\n", id_choosen); break;
        }
        //sleep(2);
        //return 0;
    }
    pthread_exit(NULL);
    return 0;
}

int main(int argc, char** argv ){
	//try{ 
        enable_emergency_stop();
        srand((unsigned) time(NULL));
        setup();
        cout<<"Program has started"<<endl;
        #ifdef raspberry
        pthread_t threads[NUM_THREADS];
        id_threads  = pthread_create(&threads[0], NULL, menu, (void *)0);
        std::string name_log = str_datetime(); 
        log_file.open("logs/"+name_log+".txt");

        id_threads_log  = pthread_create(&threads[1], NULL, logging, (void *)0);
        cout<<"Threads created "<<endl;
        #endif
        /*
        while(1){
            #ifdef raspberry
            float value;
            cin>>value;
            if(cin.fail()) throw 505;
            rasp_i2c.sendFloat(M1_VAL, value);
            cout<<"Value sent : "<<endl;
            cout<<"Value confirm : "<<rasp_i2c.readFloat(M1_VAL)<<endl;
            #endif
        }*/

        while(1){
            #ifdef raspberry
            if(!cin_thread){
                std::cin.clear();
                cout<<"id function : "<<endl;
                cin>>id_choosen;
                if(cin.fail()) throw 505;
                cout<<"function choosen: "<<id_choosen<<endl;
                cin_thread=true;
                inputReceived = true;
            #endif
            }
        }
    //}
	/*catch (...){
        normalStop();
		cout << "Error exception occurred!" << endl;
		return 0;
	}*/
    return 0;
}
