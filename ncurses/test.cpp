#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <ncurses.h>
#include "scroll_menu.h"

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
 
int main(void) {
    
    std::cout << "──" << std::endl;
    getch();
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

	WINDOW * mainwin = newwin(max_y-2*padd_y, max_x - 2*padd_x, padd_y, padd_x);
	refresh();
    wrefresh(mainwin);
    
    keypad(stdscr, true);
    keypad(mainwin, true);
    
    
    wattron(mainwin, COLOR_PAIR(1));
	box(mainwin, 0, 0);
    wattroff(mainwin, COLOR_PAIR(1));

    std::string title = "Mi primer menu";
    
    
    attron(COLOR_PAIR(2));
    mvprintw(padd_y - 3, max_x/2  - title.length()/2, title.c_str());
    mvprintw(max_y - padd_y + 1, padd_x + 5, "[S] Start");
    mvprintw(max_y - padd_y + 1, max_x - padd_x - 5 - 13, "[E] Emergency");

    attroff(COLOR_PAIR(2));
    
    

    menu arr_menu[6] = {
        menu("SendPID"),
        menu("SensorData"),
        menu("Setpoint"),
        menu("ReadRegister"),
        menu("ZeroPosition"),
        menu("Compensation")
        //menu("holssssssssssssssssa2"),
        //menu("hossssssssssssssla3"),
        //menu("hsssssssssssssssssola4"),
       /* menu("hossssssssssssssla3")*/};

    scrollMenu scm = scrollMenu(mainwin, arr_menu, 6, max_x - 2*padd_x, max_y-2*padd_y);

    scm.draw();

    

    while(1){
        
        for(int i = 0 ; i < 6; i++){
            mvwprintw(mainwin, max_y/2 - padd_y + i - 3, max_x/2 - name[i].length()/2 - padd_x, name[i].c_str());
        }
        
        wrefresh(mainwin);
    
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
    