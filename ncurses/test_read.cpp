#include "read_write.h"


using namespace std;
char bigtext[100][50];


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
    //hide_panel(workpanel);
    
    keypad(stdscr, true);
    
    wattron(mainwin, COLOR_PAIR(1));
	wborder(mainwin, '|', '|', '-', '-', '+', '+', '+', '+');
    wattroff(mainwin, COLOR_PAIR(1));
    int x, y;
    getbegyx(mainwin, y, x);
    printw("%d %d\n",x, y); 

    refresh();
    
    float var[] = {150.1212, 0.2, 0.3, 0.4};
    string names[] = {"hola", "como", "estas", "?"};
    writeData(mainpanel, names, var, 4);
    wrefresh(mainwin);
    
    update_panels();
	doupdate();

    while(1){
        
        for(int i = 0; i < 4; i++) printw("%f, ", var[i]);
        printw("\n");
    
        int c = getch();
    


        if(c == 'q') break;
        
    }

	endwin();
	return 0;

}
    
