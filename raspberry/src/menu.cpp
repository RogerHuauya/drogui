#include "menu.h"
#include <fstream>
#include <string>
bool logging_state = false;
void desplazamiento(){}

void dataSensor(){
    cin_thread = true;
    int reg;
    cls();
    printf("\t\t\t\t\t\t\t\t" blue(Sensors) "\n");
    printf(green([0]) " " white(Main menu\n));
    printf(green([1]) " " white(IMU\n));
    printf(green([2]) " " white(GPS - DSPIC\n));
    printf(green([3]) " " white(GPS - GSM\n));
    printf(green([4]) " " white(BMP280\n));
    printf(green([5]) " " white(Z value\n));
    printf(green([6]) " " white(IMU CALIBRATION\n));
    

    std::cin >> reg;
    if(std::cin.fail()) throw 505;
    if(reg == 0) {cin_thread = false; return;}
    for(int i = 0 ; i < 100 ; i++){
        switch(reg){
            case 1: std::cout << rasp_i2c.readFloat(ROLL_VAL) <<"\t" ;
                    std::cout << rasp_i2c.readFloat(PITCH_VAL) << "\t";
                    std::cout << rasp_i2c.readFloat(YAW_VAL) << std::endl; break;

            case 2: std::cout << rasp_i2c.readFloat(GPS_X) << "\t";
                    std::cout << rasp_i2c.readFloat(GPS_Y) << "\t";
                    std::cout << rasp_i2c.readFloat(X_VAL) << "\t";
                    std::cout << rasp_i2c.readFloat(Y_VAL) << std::endl; break;

            case 3: printf("Lat: %.6lf,\tLong: %.6lf,\tAlt: %.6lf\tVel: %.6f\tCurso: %.6f\n",
			sim7600.Lat,sim7600.Log,sim7600.Alt,sim7600.Vel,sim7600.Curso);break;

            case 4: std::cout << rasp_i2c.readFloat(RAW_TEMP) << "\t";
                    std::cout << rasp_i2c.readFloat(TEMP_ABS) << "\t";
                    std::cout << rasp_i2c.readFloat(RAW_PRESS) << "\t";
                    std::cout << rasp_i2c.readFloat(PRESS_ABS) << std::endl; break;

            case 5: std::cout << rasp_i2c.readFloat(Z_VAL) <<std::endl; break;

            case 6: std::cout << rasp_i2c.readFloat(CAL_SYS) <<"\t" ;
                    std::cout << rasp_i2c.readFloat(CAL_GYR) << "\t";
                    std::cout << rasp_i2c.readFloat(CAL_ACC) << "\t";
                    std::cout << rasp_i2c.readFloat(CAL_MAG) << std::endl; break;

        }
        unistd::usleep(20000);
    }
    cin_thread = false;
    return;
}

void zeroPosition(){
    return;
}

void handler_stop(int s){
    rasp_i2c.sendFloat(Z_REF, 0);
    printf("Emergency exit CTRL+C - Caught signal %d ... turning off motors\n",s);
    exit(1); 
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
    unistd::sleep(1);

    cin_thread=false;
    return;
}
void readRegister(){
    cin_thread=true;
    cls(); 
    int reg;
    
    printf("\t\t\t\t\t\t\t\t" blue(Read register) "\n");
    printf(green([0]) " " white(Main menu\n));
    printf(green([1]) " " white(H_dH\n));
    printf(green([2]) " " white(PID_ROLL\n));
    printf(green([3]) " " white(PID_PITCH\n));
    printf(green([4]) " " white(PID_YAW\n));
    printf(green([5]) " " white(SETPOINTS\n));

    std::cin >> reg;
    if(std::cin.fail()) throw 505;
    if(reg == 0) {cin_thread = false; return;}
    switch(reg){
        case 1: std::cout << rasp_i2c.readFloat(Z_REF) << " " << rasp_i2c.readFloat(Z_REF_SIZE) << std::endl; break;
        case 2: std::cout << rasp_i2c.readFloat(ROLL_KP) << " ";
                 std::cout << rasp_i2c.readFloat(ROLL_KI) << " ";
                 std::cout << rasp_i2c.readFloat(ROLL_KD) << std::endl; break;
        
        case 3: std::cout << rasp_i2c.readFloat(PITCH_KP) << " ";
                 std::cout << rasp_i2c.readFloat(PITCH_KI) << " ";
                 std::cout << rasp_i2c.readFloat(PITCH_KD) << std::endl; break;
        
        case 4:std::cout << rasp_i2c.readFloat(YAW_KP) << " ";
                 std::cout << rasp_i2c.readFloat(YAW_KI) << " ";
                 std::cout << rasp_i2c.readFloat(YAW_KD) << std::endl; break;

        case 5:std::cout << rasp_i2c.readFloat(ROLL_REF) << " ";
                 std::cout << rasp_i2c.readFloat(PITCH_REF) << " ";
                 std::cout << rasp_i2c.readFloat(YAW_REF) << std::endl; break;
        
    }

    unistd::sleep(1);
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
    unistd::sleep(1);
    cin_thread=false;
    return; 
}
void send_AT_command(){
    cin_thread = true;
    cls();
    std::string at_command;
    delay(100);
    while (Serial.available() > 0) Serial.read();    // Clean the input buffer
    while(1){
        printf("Please input the AT command: \n>>>");
        std::cin >> at_command;
	    std::cout << at_command;
        if(at_command[0] == '0') break;
        sim7600.sendATcommand(at_command.c_str(), 2000, true);
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

int start = 0;
void startSystem(){

    
    if(sim7600.GPSGet()){
        sim7600.offset_Log = sim7600.Log;
        sim7600.offset_Lat = sim7600.Lat;
    }
    unistd::sleep(1);

    start = 1 - start;
    rasp_i2c.sendFloat(START, start);
    cin_thread = false;
}

void startLogging(){
    logging_state = true;
    cin_thread=false;

}
void menu(){
    rasp_i2c.sendFloat(START, 0);

    while(1){
        if(!cin_thread){
            cls(); 
            cls();   
            printf("\t\t\t\t\t\t\t\t" blue(Principal menu) "\n");
            printf(green([1]) " " white(Desplazamiento\n));
            printf(green([2]) " " white(Show data sensor\n));
            printf(green([3]) " " white(Send PID\n));
            printf(green([4]) " " white(Zero position \n));
            printf(green([5]) " " white(Send reference \n));
            printf(green([6]) " " white(Send Compensation\n));
            printf(green([7]) " " white(Write register \n));
            printf(green([8]) " " white(Read register \n));
            printf(green([9]) " " white(Send AT command \n));
            printf(green([10]) " " white(GPS position \n));
            printf(green([11]) " " white(Send setpoint \n));
            printf(green([12]) " " white(Start Logging \n));
            if(start == 1)	printf(green([13]) " " white(Stop\n));
            else		printf(green([13]) " " white(Start\n));

            printf(white(Enter an option = \n));
            std::string aux; bool flag_aux = false;
            std::cin >> aux;

            for(int i = 0; i < aux.size() ; i++){
                if(aux[i] < '0' || aux[i] > '9') flag_aux = true;
            }

            if(flag_aux) id_choosen = 0;
            else id_choosen = std::stoi(aux);

            cin_thread = true;
            std::cout << "menu : "<<id_choosen<<std::endl;
            //unistd::sleep(1);
            switch(id_choosen){
                case 1: desplazamiento(); break;
                case 2: dataSensor(); break;
                case 3: send_PID(); break;
                case 4: zeroPosition(); break;
                case 5: send_ref();break;
                case 6: send_comp_mg(); break;
                case 7: writeRegister(); break;
                case 8: readRegister(); break;
                case 9: send_AT_command(); break;
                case 10: getGPSdata(); break;
                case 11: break;//send_setpoint(); break;
                case 12: startLogging(); break;//send_setpoint(); break;
		        case 13: startSystem(); break;
                default: printf("%d is not an option, please enter option again\n", id_choosen); cin_thread = false; break;
            }
        }
    }
}
