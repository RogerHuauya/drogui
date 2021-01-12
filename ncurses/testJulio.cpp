#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ncurses.h>

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


char bigtext[100][50];
 
int main(void) {
 	
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
    
    

    for(int i = 0 ; i < 50 ; i++)
	    sprintf(bigtext[i], "Opcion%02d", i);


	int ini = 0, fin = max_y-2*padd_y - 2, selected = 0, len = fin - ini;

    int n = 5, pos = 1;
    while(1){
        pos = 1;
        for(int i = 0; i < n; i++, pos += 11){
            
            if(i == selected){
                wattron(mainwin, A_REVERSE | COLOR_PAIR(1));
                mvwprintw(mainwin, 0, pos, bigtext[i]);
                wattroff(mainwin, A_REVERSE | COLOR_PAIR(1));
            }
            else mvwprintw(mainwin, 0, pos, bigtext[i]);
        }
        //printw(name[0].c_str());

        for(int i = 0 ; i < 11; i++){
            mvwprintw(mainwin, max_y/2 - padd_y + i - 6, max_x/2 - name[i].length()/2 - padd_x, name[i].c_str());
        }
        
        wrefresh(mainwin);

        int c = getch();
        switch(c){
            case KEY_LEFT:
                selected --;
                break;
            case KEY_RIGHT:
                selected ++;
                break;
        }

        selected = (selected + 5)%5;

        if(selected < ini) ini = selected, fin = ini + len;
        if(selected >= fin) fin = selected + 1, ini = fin - len;

        if( c == 'q' ) break;
    }

	endwin();
	return 0;

}
    