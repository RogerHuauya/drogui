#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "scroll_menu.h"
#include <unistd.h>
#include "read_write.h"


std::string name[] = {  
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


std::string name2[]{
"    .___                          .__ ",
"  __| _/______  ____   ____  __ __|__|",
" / __ |\\_  __ \\/  _ \\ / ___\\|  |  \\  |",
"/ /_/ | |  | \\(  <_> ) /_/  >  |  /  |",
"\\____ | |__|   \\____/\\___  /|____/|__|",
"     \\/             /_____/           "};


char bigtext[100][50];



string pid_op[] = {"PID ROLL", "PID PITCH", "PID YAW" ,"PID X" ,"PID Y" , "PID Z"};
bool pidOp(PANEL* pan, int index){
    if(index < 3){    
        string names[] = {"index", "KP", "KD", "KI"};
        float arr[4]; 
        if(readData(pan, pid_op[index], names, arr, 4)){

        }
    }
    else{
        string names[] = {"KP", "KD", "KI"};
        float arr[3]; 
        if(readData(pan, pid_op[index], names, arr, 3)){

        }
    }
    return true;
}


string sensor_data_op[] = {"IMU", "GPS", "IMU CAL", "HEIGHT"};
bool sensorDataOp(PANEL* pan, int index){
    if(index == 0){    
        string names[] = {"roll", "pitch", "yaw"};
        float arr[] = {0.012, 0.015, -3.1415}; 
        writeData(pan, sensor_data_op[index], names, arr, 3);
    }
    else if(index == 1){
        string names[] = {"GSM_X", "GSM_Y", "X", "Y"};
        float arr[] = {0.012, 0.015, 0.002, 0.013}; 
        writeData(pan, sensor_data_op[index], names, arr, 4);
    }
    else if(index == 2){
        string names[] = {"SYS", "GYR", "ACC", "MAG"};
        float arr[] = {3, 3, 3, 2}; 
        writeData(pan, sensor_data_op[index], names, arr, 4);
    }
    else if(index == 3){
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
        if(readData(pan, pid_op[index], names, arr, 1)){

        }
    }
    else{
        string names[] = {"meters"};
        float arr[1]; 
        if(readData(pan, pid_op[index], names, arr, 1)){

        }
    }
    return true;
}
string various_op[] = {"Read Register", "Write Register", "Zero Pos", "Compenstation"}; 
bool variousOp(PANEL* pan, int index){
    //wmove(win, 5, 5);
    //wprintw(win, "kha3");

    return true;
}

 
int main(void) {
    
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
        menu("SensorData", sensor_data_op, 4, &sensorDataOp),
        menu("Setpoint", setpoint_op, 6, &setpointOp),
        menu("Various", various_op, 4, &variousOp)
    };

    scrollMenu scm = scrollMenu(mainpanel, workpanel, arr_menu, 4);

    scm.draw();
    
    for(int i = 0 ; i < 11; i++){
        mvwprintw(mainwin, max_y/2 - padd_y + i - 6, max_x/2 - name[i].length()/2 - padd_x, name[i].c_str());
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
    