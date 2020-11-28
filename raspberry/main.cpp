#define raspberry
#define BUFF_LENGTH 500
#define DSPIC_ADDRESS 0x60
#define pi 3.141592
#include <iostream>
#include "registerMap.h"
#include "arduPi.h"
#include "sim7600.h"
#include <ctime>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#ifdef raspberry
#include "utils.h"
#include "control.h"
#include <pthread.h>
rasp_I2C rasp_i2c(DSPIC_ADDRESS);
#endif
#define POWERKEY 6

bool inputReceived = false, logging_state = false;
bool cin_thread = false;
int id_choosen, value; 
int id_thread_menu, id_thread_log, id_thread_gps;

std::ofstream log_file;

void setup() {
	sim7600.PowerOn(POWERKEY);
}

void sleep(unsigned milliseconds){
    unistd::usleep(milliseconds * 1000); // takes microseconds
}

void desplazamiento(){}

void dataSensor(){

    printf("\t\t\t\t\t\t\t\t" blue(Sensors) "\n");
    printf(green([0]) " " white(IMU\n));
    printf(green([1]) " " white(GPS\n));
    printf(green([2]) " " white(BMP280\n));

    std::cin >> reg;
    if(std::cin.fail()) throw 505;
    while(1){
        switch(reg){
            case 0: std::cout << rasp_i2c.readFloat(ROLL_VAL) <<" " ;
                    std::cout << rasp_i2c.readFloat(PITCH_VAL) << " ";
                    std::cout << rasp_i2c.readFloat(YAW_VAL) << std::endl; break;

            case 1: std::cout << rasp_i2c.readFloat(GPS_X) << " ";
                    std::cout << rasp_i2c.readFloat(GPS_Y) << std::endl; break;
            
            case 2: std::cout << rasp_i2c.readFloat(RAW_TEMP) << " ";
                    std::cout << rasp_i2c.readFloat(TEMP_ABS) << " ";
                    std::cout << rasp_i2c.readFloat(RAW_PRESS) << " ";
                    std::cout << rasp_i2c.readFloat(PRESS_ABS) << std::endl; break;
        }
    }
    return;
}

void zeroPosition(){
    return;

}
void normalStop(){
    rasp_i2c.sendFloat(Z_REF, 0);
	return;
}
void handler_stop(int s){
    normalStop();
    printf("Emergency exit CTRL+C - Caught signal %d ... turning off motors\n",s);
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

void writeRegister(){
    cin_thread=true;
    cls(); 
    int reg;
    float value;
    printf("REG  FLOAT = \n");
    std::cin>>reg>>value;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat((uint8_t)reg, value);
    printf("Value sent : %.3f\n", value);
    printf("Value retrieved : %.3f\n", rasp_i2c.readFloat(reg));
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

    std::cin >> reg;
    if(std::cin.fail()) throw 505;
    switch(reg){
        case 0: std::cout << rasp_i2c.readFloat(H_VAL) << " " << rasp_i2c.readFloat(H_STEP_SIZE) << std::endl; break;
        case 1: std::cout << rasp_i2c.readFloat(ROLL_KP) << " ";
                 std::cout << rasp_i2c.readFloat(ROLL_KI) << " ";
                 std::cout << rasp_i2c.readFloat(ROLL_KD) << std::endl; break;
        
        case 2: std::cout << rasp_i2c.readFloat(PITCH_KP) << " ";
                 std::cout << rasp_i2c.readFloat(PITCH_KI) << " ";
                 std::cout << rasp_i2c.readFloat(PITCH_KD) << std::endl; break;
        
        case 3:std::cout << rasp_i2c.readFloat(YAW_KP) << " ";
                 std::cout << rasp_i2c.readFloat(YAW_KI) << " ";
                 std::cout << rasp_i2c.readFloat(YAW_KD) << std::endl; break;

        case 4:std::cout << rasp_i2c.readFloat(ROLL_REF) << " ";
                 std::cout << rasp_i2c.readFloat(PITCH_REF) << " ";
                 std::cout << rasp_i2c.readFloat(YAW_REF) << std::endl; break;
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
    printf("Ts = \n");
    std::cin>>value1;
    if(std::cin.fail()) throw 505;
    rasp_i2c.sendFloat(TS_CONTROL, value1);
    printf("Values sent : \n");
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
        std::string name_log = str_datetime(); 
        log_file.open("logs/"+name_log+".txt");
        /*
        log_file << "H_VAL   H_STEP_SIZE " << rasp_i2c.readFloat(H_VAL) << " " << rasp_i2c.readFloat(H_STEP_SIZE)<< std::endl;
        log_file << "ROLL KP KI KD " << rasp_i2c.readFloat(ROLL_KP) << " " <<rasp_i2c.readFloat(ROLL_KI) << " " <<rasp_i2c.readFloat(ROLL_KD)<<std::endl;
        log_file << "PITCH KP KI KD " << rasp_i2c.readFloat(PITCH_KP) << " " <<rasp_i2c.readFloat(PITCH_KI) << " " <<rasp_i2c.readFloat(PITCH_KD)<<std::endl;
        log_file << "YAW KP KI KD " << rasp_i2c.readFloat(YAW_KP) << " " <<rasp_i2c.readFloat(YAW_KI) << " " <<rasp_i2c.readFloat(YAW_KD)<<std::endl;
        */
        while(1){
            /*
            log_file<<rasp_i2c.readFloat(H_VAL);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(ROLL_DEG)*180.0/pi+180;
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(PITCH_DEG)*180.0/pi+180;
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(YAW_DEG)*180.0/pi+180;
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(ROLL_REF);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(PITCH_REF);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(YAW_REF);
	    log_file<<"\t";
	    log_file<<rasp_i2c.readFloat(PRESS_ABS) << std::endl;
        */
            log_file<<rasp_i2c.readFloat(RAW_TEMP);
            log_file<<"\t";           
            log_file<<rasp_i2c.readFloat(TEMP_ABS);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(RAW_PRESS);
            log_file<<"\t";
            log_file<<rasp_i2c.readFloat(PRESS_ABS);
            log_file<<std::endl;
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
        while(!inputReceived){};
        inputReceived = false;
        std::cout << "menu : "<<id_choosen<<std::endl;
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
            case 13: break;//send_setpoint(); break;
            default: printf("%d is not an option, please enter option again\n", id_choosen); break;
        }
    }
    pthread_exit(NULL);
    return 0;
}

void *gps_data(void *threadid){
    sim7600.GPSStart();
    float offset_x = 0, offset_y = 0, r = 1;
    for(int i = 0; i < 10; i++){
        if(sim7600.GPSGet()){
            offset_x += r*sim7600.Log*cos(sim7600.Lat*pi/180);
            offset_y += r*sim7600.Lat;
        }
        offset_x /= 10;
        offset_y /= 10;
    }
    while(1){
        if(sim7600.GPSGet()){
            rasp_i2c.sendFloat(GPS_X, r*sim7600.Log*cos(sim7600.Lat*pi/180) - offset_x);
            rasp_i2c.sendFloat(GPS_Y, r*sim7600.Lat - offset_y);
        }
        delay(100);
    }
    return;
}
int main(int argc, char** argv ){
        enable_emergency_stop();
        srand((unsigned) time(NULL));
        setup();
        printf("Program has started\n");
        #ifdef raspberry
        pthread_t threads[NUM_THREADS];
        id_thread_menu  = pthread_create(&threads[0], NULL, menu, (void *)0);
        id_thread_log  = pthread_create(&threads[1], NULL, logging, (void *)0);
        id_thread_gps = pthread_create(&threads[2], NULL, gps_data, (void *)0);
        printf("Threads created \n");
        #endif


        while(1){
            #ifdef raspberry
            if(!cin_thread){
                std::cin.clear();
                printf("id function : \n");
                std::cin>>id_choosen;
                if(std::cin.fail()) throw 505;
                printf("function choosen: \n");
                cin_thread=true;
                inputReceived = true;
            }
            #endif
        }

    return 0;
}
