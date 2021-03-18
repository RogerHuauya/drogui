#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "scroll_menu.h"
#include <unistd.h>
#include "read_write.h"
#include "curmenu.h"
#include <time.h>

extern rasp_I2C rasp_i2c;
bool logging_state = false;

std::string name2[] = {  
"     _____          ___           ___           ___           ___                 ",
"    /  /::\\        /  /\\         /  /\\         /  /\\         /__/\\        ___     ",
"   /  /:/\\:\\      /  /::\\       /  /::\\       /  /:/_        \\  \\:\\      /  /\\    ",
"  /  /:/  \\:\\    /  /:/\\:\\     /  /:/\\:\\     /  /:/ /\\        \\  \\:\\    /  /:/    ",
" /__/:/ \\__\\:|  /  /:/~/:/    /  /:/  \\:\\   /  /:/_/::\\   ___  \\  \\:\\  /__/::\\    ",
" \\  \\:\\ /  /:/ /__/:/ /:/___ /__/:/ \\__\\:\\ /__/:/__\\/\\:\\ /__/\\  \\__\\:\\ \\__\\/\\:\\__ ",
"  \\  \\:\\  /:/  \\  \\:\\/:::::/ \\  \\:\\ /  /:/ \\  \\:\\ /~~/:/ \\  \\:\\ /  /:/    \\  \\:\\/\\",
"   \\  \\:\\/:/    \\  \\::/~~~~   \\  \\:\\  /:/   \\  \\:\\  /:/   \\  \\:\\  /:/      \\__\\::/",
"    \\  \\::/      \\  \\:\\        \\  \\:\\/:/     \\  \\:\\/:/     \\  \\:\\/:/       /__/:/ ",
"     \\__\\/        \\  \\:\\        \\  \\::/       \\  \\::/       \\  \\::/        \\__\\/  ",
"                   \\__\\/         \\__\\/         \\__\\/         \\__\\/                "};


std::string name[]{
"    .___                          .__ ",
"  __| _/______  ____   ____  __ __|__|",
" / __ |\\_  __ \\/  _ \\ / ___\\|  |  \\  |",
"/ /_/ | |  | \\(  <_> ) /_/  >  |  /  |",
"\\____ | |__|   \\____/\\___  /|____/|__|",
"     \\/             /_____/           "};


char bigtext[100][50];

void handler_stop(int s){
    rasp_i2c.sendFloat(Z_REF, 0);
    //printf("Emergency exit CTRL+C - Caught signal %d ... turning off motors\n",s);
    rasp_i2c.finish();
    endwin();
    exit(1); 
}


string pid_op[] = {"PID ROLL", "PID PITCH", "PID YAW" ,"PID X" ,"PID Y" , "PID Z", "N FILTER"};
bool pidOp(PANEL* pan, int index){
    if(index < 3){    
        string names[] = {"index", "KP", "KI", "KD"};
        float arr[4]; 
        if(readData(pan, pid_op[index], names, arr, 4)){
            switch (index){
                case 0: rasp_i2c.sendFloat(PID_INDEX, arr[0]);
                        rasp_i2c.sendFloat(PID_VAR, PID_ROLL);
                        rasp_i2c.sendFloat(ROLL_KP, arr[1]);
                        rasp_i2c.sendFloat(ROLL_KI, arr[2]);
                        rasp_i2c.sendFloat(ROLL_KD, arr[3]);
                        break;
                case 1: rasp_i2c.sendFloat(PID_INDEX, arr[0]);
                        rasp_i2c.sendFloat(PID_VAR, PID_PITCH);
                        rasp_i2c.sendFloat(PITCH_KP, arr[1]);
                        rasp_i2c.sendFloat(PITCH_KI, arr[2]);
                        rasp_i2c.sendFloat(PITCH_KD, arr[3]);
                        break;

                case 2: rasp_i2c.sendFloat(PID_INDEX, arr[0]);
                        rasp_i2c.sendFloat(PID_VAR, PID_YAW);
                        rasp_i2c.sendFloat(YAW_KP, arr[1]);
                        rasp_i2c.sendFloat(YAW_KI, arr[2]);
                        rasp_i2c.sendFloat(YAW_KD, arr[3]);
                        break;
            }
        }
    }
    else if(index < 6){
        string names[] = {"KP", "KI", "KD"};
        float arr[3]; 
        if(readData(pan, pid_op[index], names, arr, 3)){
            switch (index){
                case 3: rasp_i2c.sendFloat(PID_VAR, PID_X);
                        rasp_i2c.sendFloat(X_KP, arr[1]);
                        rasp_i2c.sendFloat(X_KI, arr[2]);
                        rasp_i2c.sendFloat(X_KD, arr[3]);
                        break;
                case 4: rasp_i2c.sendFloat(PID_VAR, PID_Y);
                        rasp_i2c.sendFloat(Y_KP, arr[1]);
                        rasp_i2c.sendFloat(Y_KI, arr[2]);
                        rasp_i2c.sendFloat(Y_KD, arr[3]);
                        break;
                case 5: rasp_i2c.sendFloat(PID_VAR, PID_Z);
                        rasp_i2c.sendFloat(Z_KP, arr[1]);
                        rasp_i2c.sendFloat(Z_KI, arr[2]);
                        rasp_i2c.sendFloat(Z_KD, arr[3]);
                        break;
            }

        }
    }
    else{
	string names[] = {"N FILTER"};
        float arr[1];
        if(readData(pan, pid_op[index], names, arr, 1)){
            rasp_i2c.sendFloat(N_FILTER, arr[0]);
        }
    }

    return true;
}


string sensor_data_op[] = {"IMU", "GPS", "KALMAN", "IMU CAL", "HEIGHT"};
bool sensorDataOp(PANEL* pan, int index){
    if(index == 0){    
        string names[] = {"roll", "pitch", "yaw"};
        float arr[] = { rasp_i2c.readFloat(ROLL_VAL) , rasp_i2c.readFloat(PITCH_VAL), rasp_i2c.readFloat(YAW_VAL)}; 
        writeData(pan, sensor_data_op[index], names, arr, 3);
    }
    else if(index == 1){
        string names[] = {"latitud", "longitud"};
        float arr[] = {(float)sim7600.Lat,(float) sim7600.Log}; 
        writeData(pan, sensor_data_op[index], names, arr, 2);
    }
    else if(index == 2){
        string names[] = {"GSM_X", "GSM_Y", "X", "Y"};
        float arr[] = {rasp_i2c.readFloat(GPS_X), rasp_i2c.readFloat(GPS_Y), rasp_i2c.readFloat(X_VAL), rasp_i2c.readFloat(Y_VAL)}; 
        //printw("%f %f %f %f\n", arr[0], arr[1], arr[2], arr[3]);
	//refresh();
	writeData(pan, sensor_data_op[index], names, arr, 4);
    }
    else if(index == 3){
        string names[] = {"SYS", "GYR", "ACC", "MAG"};
        float arr[] = {rasp_i2c.readFloat(CAL_SYS), rasp_i2c.readFloat(CAL_GYR), rasp_i2c.readFloat(CAL_ACC), rasp_i2c.readFloat(CAL_MAG)}; 
        writeData(pan, sensor_data_op[index], names, arr, 4);
    }
    else if(index == 4){
        string names[] = {"Z"};
        float arr[] = {rasp_i2c.readFloat(Z_VAL)}; 
        writeData(pan, sensor_data_op[index], names, arr, 1);
    }

    return true;
}


string setpoint_op[] = {"ROLL", "PITCH", "YAW", "X", "Y", "Z"};
bool setpointOp(PANEL* pan, int index){
    if(index < 3){    
        string names[] = {"degrees"};
        float arr[1]; 
        if(readData(pan, setpoint_op[index], names, arr, 1)){
            arr[0] *= pi/180;
            switch(index){
                
                case 0:
                    rasp_i2c.sendFloat(ROLL_REF,arr[0]); break;
                case 1: 
                    rasp_i2c.sendFloat(PITCH_REF,arr[0]); break;
                case 2:
                    rasp_i2c.sendFloat(YAW_REF,arr[0]); break;
            }

        }
    }
    else{
        string names[] = {"meters"};
        float arr[1]; 
        if(readData(pan, setpoint_op[index], names, arr, 1)){
            switch(index){
           
                case 3:
                    rasp_i2c.sendFloat(X_REF,arr[0]); break;    
                case 4:
                    rasp_i2c.sendFloat(Y_REF,arr[0]); break;
                case 5:
                    rasp_i2c.sendFloat(Z_REF,arr[0]); break;
            }
        }
    }
    return true;
}

bool start = false;

string various_op[] = {"Read Register", "Write Register", "Start kalman", "Compensation", "Start logging", "Update Offset", "Noise H parameters"}; 
bool variousOp(PANEL* pan, int index){
    //wmove(win, 5, 5);
    //wprintw(win, "kha3");
    if(index == 6){
        string names[] = {"Amplitude", "Frecuency (Hz.)"};
        float arr[2];
        if(readData(pan, various_op[index], names, arr, 2)){
            rasp_i2c.sendFloat(AMP_SIN, arr[0]);
            rasp_i2c.sendFloat(FREQ_SIN, arr[1]);
        }
    }
    else if(index == 5){
        string names[] = {"roll", "pitch", "yaw"};

        float offset_roll = 0;
        float offset_pitch = 0;
        float offset_yaw = 0;

        for( int i = 0; i < 50; i++ ){
            offset_roll += rasp_i2c.readFloat(ROLL_VAL);
            usleep(10000);
	        offset_pitch += rasp_i2c.readFloat(PITCH_VAL);
	        usleep(10000);
	        offset_yaw += rasp_i2c.readFloat(YAW_VAL);
            usleep(10000);
	    } 

        offset_roll /= 50.0;
        offset_pitch /= 50.0;
        offset_yaw /= 50.0;

	    offset_roll += rasp_i2c.readFloat(ROLL_OFFSET);
        offset_pitch += rasp_i2c.readFloat(PITCH_OFFSET);
        offset_yaw += rasp_i2c.readFloat(YAW_OFFSET);

        rasp_i2c.sendFloat(ROLL_OFFSET,offset_roll);
        rasp_i2c.sendFloat(PITCH_OFFSET,offset_pitch);
        rasp_i2c.sendFloat(YAW_OFFSET,offset_yaw);

        std::fstream offsetfile;
        offsetfile.open("../rpicurses/memory/offset_angles.txt",std::ios::out);{
        if(offsetfile.is_open()) 
            offsetfile << offset_roll << "\t" << offset_pitch << "\t" << offset_yaw; 
            offsetfile.close();
        }

        float arr[] = { offset_roll, offset_pitch, offset_yaw }; 
        writeData(pan, various_op[index], names, arr, 3);
    }
    else if(index == 4){
        logging_state = !logging_state;
        various_op[index] = (logging_state ? "Stop  logging":"Start logging");
    }
    else if(index == 3){
        string names[] = {"0 - 100 %"};
        float arr[1]; 
        if(readData(pan, various_op[index], names, arr, 1)){
            rasp_i2c.sendFloat(Z_MG, arr[0]);
	}
    }
    else if (index == 2){
        if(sim7600.GPSGet()){
            sim7600.offset_Log = sim7600.Log;
            sim7600.offset_Lat = sim7600.Lat;
        }
        start = !start;
        rasp_i2c.sendFloat(START, start);
        various_op[index] = (start ? "Stop  Kalman":"Start Kalman");
    }
    else if (index == 0){
        string names[] = {"register"};
        float arr[1];
        if(readData(pan, various_op[index], names, arr, 1)){
            arr[0] = rasp_i2c.readFloat((uint8_t) arr[0]);
            names[0] = "value";
            writeData(pan, various_op[index], names, arr, 1);
        }
    }
    else{
        
        string names[] = {"register"};
        float arr[1];
        if(readData(pan, various_op[index], names, arr, 1)){
            float reg = arr[0];
            names[0] = "value";
            if(readData(pan, various_op[index], names, arr, 1)){
                rasp_i2c.sendFloat(reg,arr[0]);
            }
        }
    }


    return true;
}




string calibration_op[] = {"GYRO", "ACC", "MAG"};

bool calibrationOp(PANEL* pan, int index){
    float arr[1];
    if(index == 0){
        string names[] = {"trigger"};
        if(readData(pan, calibration_op[index], names, arr, 1)){
            rasp_i2c.sendFloat(CAL_GYR_TRG,arr[0]);
        }
        sleep(1);
        while(rasp_i2c.readFloat(CAL_GYR) < 100.0)
            usleep(100000);

        std::fstream fil;
        fil.open("../rpicurses/memory/cal_gyr.txt",std::ios::out);
        if(fil.is_open()){ 
            fil << rasp_i2c.readFloat(GYR_X_OFF) << "\t"\
                << rasp_i2c.readFloat(GYR_Y_OFF) << "\t"\
                << rasp_i2c.readFloat(GYR_Z_OFF) << std::endl; 
            fil.close();
        }
    }
    else if(index == 1){
        string names[] = {"trigger"};
        if(readData(pan, calibration_op[index], names, arr, 1)){
            rasp_i2c.sendFloat(CAL_ACC_TRG,arr[0]);
        }
        sleep(1);
        while(rasp_i2c.readFloat(CAL_ACC) < 100.0)
            usleep(100000);

        std::fstream fil;
        fil.open("../rpicurses/memory/cal_acc.txt",std::ios::out);
        if(fil.is_open()){ 
            fil << rasp_i2c.readFloat(ACC_X_OFF) << "\t"\
                << rasp_i2c.readFloat(ACC_Y_OFF) << "\t"\
                << rasp_i2c.readFloat(ACC_Z_OFF) << "\t"\
                << rasp_i2c.readFloat(ACC_SCALE) << std::endl; 
            fil.close();
        }
    }
    else if(index == 2){
	string names[] = {"trigger"};
        if(readData(pan, calibration_op[index], names, arr, 1)){
            rasp_i2c.sendFloat(CAL_MAG_TRG,arr[0]);
        }
        sleep(1);
        while(rasp_i2c.readFloat(CAL_MAG) < 100.0);
            usleep(100000);

        std::fstream fil;
        fil.open("../rpicurses/memory/cal_mag.txt",std::ios::out);
        if(fil.is_open()){ 
            fil << rasp_i2c.readFloat(MAG_X_OFF) << "\t"\
                << rasp_i2c.readFloat(MAG_Y_OFF) << "\t"\
                << rasp_i2c.readFloat(MAG_Z_OFF) << "\t"\
                << rasp_i2c.readFloat(MAG_X_SCALE) << "\t"\
                << rasp_i2c.readFloat(MAG_Y_SCALE) << "\t"\
                << rasp_i2c.readFloat(MAG_Z_SCALE) << std::endl; 
            fil.close();
        }
    }

    return true;
}





 
int curmenu(void) {
    
    rasp_i2c.sendFloat(START, 0);
    setlocale(LC_ALL, "");
 	initscr();
	//cbreak();
	noecho();
    	curs_set(0);
	if(!has_colors()){
		
		getch();
		return -1;
	}	
	start_color();
	init_pair(1, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(2, COLOR_RED, COLOR_WHITE);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	init_pair(4, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(5, COLOR_BLACK, COLOR_WHITE);
	
	bkgd(COLOR_PAIR(3));

	int height = 20, width = 20, padd_x = 5, padd_y = 5;
	int max_x, max_y;

	getmaxyx(stdscr, max_y, max_x);

	//WINDOW * mainwin = newwin(max_y-2*padd_y, max_x - 2*padd_x, padd_y, padd_x);
	PANEL * mainpanel = new_panel(newwin(max_y-2*padd_y, max_x - 2*padd_x, padd_y, padd_x)); 
    WINDOW * mainwin = panel_window(mainpanel);
	wbkgd(mainwin, COLOR_PAIR(3));

    WINDOW * workwin = newwin(max_y-2*padd_y -2, max_x - 2*padd_x - 2, padd_y + 1, padd_x + 1);
	PANEL * workpanel = new_panel(workwin); 
    	wbkgd(workwin, COLOR_PAIR(3));
	hide_panel(workpanel);
    
    keypad(stdscr, true);
    
    wattron(mainwin, COLOR_PAIR(1));
	wborder(mainwin, '|', '|', '-', '-', '+', '+', '+', '+');
    wattroff(mainwin, COLOR_PAIR(1));

    std::string title = "Menu Principal";
    
    
    attron(COLOR_PAIR(4));
    mvprintw(padd_y - 3, max_x/2  - title.length()/2, title.c_str());
    mvprintw(max_y - padd_y + 1, padd_x + 5, "[S] Start");
    mvprintw(max_y - padd_y + 1, max_x - padd_x - 5 - 13, "[E] Emergency");

    attroff(COLOR_PAIR(4));
    
    
    
    menu arr_menu[] = {
        menu("SendPID", pid_op, 7, &pidOp),
        menu("SensorData", sensor_data_op, 5, &sensorDataOp),
        menu("Calibration", calibration_op, 3, &calibrationOp),
        menu("Setpoint", setpoint_op, 6, &setpointOp),
	menu("Various", various_op, 7, &variousOp)
    };

    scrollMenu scm = scrollMenu(mainpanel, workpanel, arr_menu, 5);

    scm.draw();
    wattron(mainwin, COLOR_PAIR(5));
    for(int i = 0 ; i < 6; i++){
        mvwprintw(mainwin, max_y/2 - padd_y + i - 3, max_x/2 - name[i].length()/2 - padd_x, name[i].c_str());
    }
	wattroff(mainwin, COLOR_PAIR(5));

    refresh();
    wrefresh(mainwin);
    update_panels();
	doupdate();
    


    while(1){
        
        
    
        int c = getch();
        scm.keyHandling(c);
    

        if(c == 'q') break;

        if(c == 's'){
            attron(COLOR_PAIR(4));
            attron(A_REVERSE);
            mvprintw(max_y - padd_y + 1, padd_x + 5, "[S] Start");
            attroff(A_REVERSE);
            mvprintw(max_y - padd_y + 1, max_x - padd_x - 5 - 13, "[E] Emergency");
            attroff(COLOR_PAIR(4));
        }
        else if(c == 'e'){
        
            attron(COLOR_PAIR(4));
            mvprintw(max_y - padd_y + 1, padd_x + 5, "[S] Start");
            attron(A_REVERSE);
            mvprintw(max_y - padd_y + 1, max_x - padd_x - 5 - 13, "[E] Emergency");
            attroff(A_REVERSE);
            attroff(COLOR_PAIR(4));
        }
        else{
            
            attron(COLOR_PAIR(4));
            mvprintw(max_y - padd_y + 1, padd_x + 5, "[S] Start");
            mvprintw(max_y - padd_y + 1, max_x - padd_x - 5 - 13, "[E] Emergency");
            attroff(COLOR_PAIR(4));
        }
        


    }
	rasp_i2c.sendFloat(Z_REF, 0);
	endwin();
	return 0;

}
    
