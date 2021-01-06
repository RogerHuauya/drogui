#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ncurses.h>


char bigtext[100][50];
 
int main(void) {
 	initscr();
	cbreak();
	noecho();
	if(!has_colors()){
		
		getch();
		return -1;
	}	
	start_color();

	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);

	int height = 20, width = 20, padd_x = 10, padd_y = 10;
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
    
    
    wattron(mainwin, COLOR_PAIR(2));
    mvwprintw(mainwin, 0, max_x/2 - padd_x - title.length()/2, title.c_str());
    wattroff(mainwin, COLOR_PAIR(2));



    for(int i = 0 ; i < 50 ; i++)
	    sprintf(bigtext[i], "hello + %02d", i);


	int ini = 0, fin = max_y-2*padd_y - 2, selected = 0, len = fin - ini;

    
    while(1){
        for(int i = ini, j = 1; i < fin; i++, j++){
            
            if(i == selected){
                wattron(mainwin, A_REVERSE | COLOR_PAIR(1));
                mvwprintw(mainwin, j, 1, bigtext[i]);
                wattroff(mainwin, A_REVERSE | COLOR_PAIR(1));
            }
            else mvwprintw(mainwin, j, 1, bigtext[i]);
        }

        wrefresh(mainwin);

        int c = getch();
        switch(c){
            case 259:
                selected --;
                break;
            case KEY_DOWN:
                selected ++;
                break;
        }

        selected = (selected + 50)%50;

        if(selected < ini) ini = selected, fin = ini + len;
        if(selected >= fin) fin = selected + 1, ini = fin - len;

        if(c == 'q') break;

    }

	endwin();
	return 0;

}
