#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "scroll_menu.h"
#include <unistd.h>
#include "read_write.h"
#include "curmenu.h"

#define pi acos(-1)


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
    exit(1); 
}


string pid_op[] = {"PID ROLL", "PID PITCH", "PID YAW" ,"PID X" ,"PID Y" , "PID Z"};
bool pidOp(PANEL* pan, int index){
    if(index < 3){    
        string names[] = {"index", "KP", "KD", "KI"};
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
    else{
        string names[] = {"KP", "KD", "KI"};
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
        float arr[] = {rasp_i2c.readFloat(GPS_X), rasp_i2c.readFloat(GPS_X), rasp_i2c.readFloat(X_VAL), rasp_i2c.readFloat(Y_VAL)}; 
        writeData(pan, sensor_data_op[index], names, arr, 4);
    }
    else if(index == 3){
        string names[] = {"SYS", "GYR", "ACC", "MAG"};
        float arr[] = {rasp_i2c.readFloat(CAL_SYS), rasp_i2c.readFloat(CAL_GYR), rasp_i2c.readFloat(CAL_ACC), rasp_i2c.readFloat(CAL_MAG)}; 
        writeData(pan, sensor_data_op[index], names, arr, 4);
    }
    else if(index == 4){
        string names[] = {"Z"};
        float arr[] = {0.012}; 
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

string various_op[] = {"Read Register", "Write Register", "Start kalman", "Compensation", "Start logging"}; 
bool variousOp(PANEL* pan, int index){
    //wmove(win, 5, 5);
    //wprintw(win, "kha3");
    if(index == 4) logging_state = true;
    else if(index == 3){
        string names[] = {"0 - 100 %"};
        float arr[1]; 
        if(readData(pan, various_op[index], names, arr, 1)){
            rasp_i2c.sendFloat(Z_MG, arr[0]);
        }
    }
    else{
        if(sim7600.GPSGet()){
            sim7600.offset_Log = sim7600.Log;
            sim7600.offset_Lat = sim7600.Lat;
        }
        rasp_i2c.sendFloat(START, 0);
        sleep(1);
        rasp_i2c.sendFloat(START, 1);
    }


    return true;
}


 
int curmenu(void) {
    
    setlocale(LC_ALL, "");
 	initscr();
	cbreak();
	noecho();
    curs_set(0);
	if(!has_colors()){
		
		getch();
		return -1;
	}	
	start_color();

	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);

	int height = 20, width = 20, padd_x = 5, padd_y = 5;
	int max_x, max_y;

	getmaxyx(stdscr, max_y, max_x);

	//WINDOW * mainwin = newwin(max_y-2*padd_y, max_x - 2*padd_x, padd_y, padd_x);
	PANEL * mainpanel = new_panel(newwin(max_y-2*padd_y, max_x - 2*padd_x, padd_y, padd_x)); 
    WINDOW * mainwin = panel_window(mainpanel);

    WINDOW * workwin = newwin(max_y-2*padd_y -2, max_x - 2*padd_x - 2, padd_y + 1, padd_x + 1);
	PANEL * workpanel = new_panel(workwin); 
    hide_panel(workpanel);
    
    keypad(stdscr, true);
    
    wattron(mainwin, COLOR_PAIR(1));
	wborder(mainwin, '|', '|', '-', '-', '+', '+', '+', '+');
    wattroff(mainwin, COLOR_PAIR(1));

    std::string title = "Menu Principal";
    
    
    attron(COLOR_PAIR(2));
    mvprintw(padd_y - 3, max_x/2  - title.length()/2, title.c_str());
    mvprintw(max_y - padd_y + 1, padd_x + 5, "[S] Start");
    mvprintw(max_y - padd_y + 1, max_x - padd_x - 5 - 13, "[E] Emergency");

    attroff(COLOR_PAIR(2));
    
    
    
    menu arr_menu[] = {
        menu("SendPID", pid_op, 6, &pidOp),
        menu("SensorData", sensor_data_op, 5, &sensorDataOp),
        menu("Setpoint", setpoint_op, 6, &setpointOp),
        menu("Various", various_op, 5, &variousOp)
    };

    scrollMenu scm = scrollMenu(mainpanel, workpanel, arr_menu, 4);

    scm.draw();
    
    for(int i = 0 ; i < 6; i++){
        mvwprintw(mainwin, max_y/2 - padd_y + i - 3, max_x/2 - name[i].length()/2 - padd_x, name[i].c_str());
    }

    refresh();
    wrefresh(mainwin);
    update_panels();
	doupdate();
    


    while(1){
        
        
    
        int c = getch();
        scm.keyHandling(c);
    

        if(c == 'q') break;

        if(c == 's'){
            attron(COLOR_PAIR(2));
            attron(A_REVERSE);
            mvprintw(max_y - padd_y + 1, padd_x + 5, "[S] Start");
            attroff(A_REVERSE);
            mvprintw(max_y - padd_y + 1, max_x - padd_x - 5 - 13, "[E] Emergency");
            attroff(COLOR_PAIR(2));
        }
        else if(c == 'e'){
        
            attron(COLOR_PAIR(2));
            mvprintw(max_y - padd_y + 1, padd_x + 5, "[S] Start");
            attron(A_REVERSE);
            mvprintw(max_y - padd_y + 1, max_x - padd_x - 5 - 13, "[E] Emergency");
            attroff(A_REVERSE);
            attroff(COLOR_PAIR(2));
        }
        else{
            
            attron(COLOR_PAIR(2));
            mvprintw(max_y - padd_y + 1, padd_x + 5, "[S] Start");
            mvprintw(max_y - padd_y + 1, max_x - padd_x - 5 - 13, "[E] Emergency");
            attroff(COLOR_PAIR(2));
        }
        


    }

	endwin();
	return 0;

}
    