
#ifndef READ_WRITE_H
#define READ_WRITE_H

#include <string>
#include <panel.h>
using namespace std;


bool readData(PANEL* pan, string title, string* names, float* arr, int sz){
    
    curs_set(1);
    WINDOW* win = panel_window(pan);
    
    int sz_x, sz_y, form = 8, tam = form*sz + sz-1, cur;
    int selected = 0;
    getmaxyx(win, sz_y, sz_x);
    
    int pos[sz];
    pos[0] = sz_x/2 - tam/2;
    for(int i = 1 ; i < sz; i++) pos[i] = pos[i-1] + 9;

    string values[] = {"0","0","0","0"};
    
    mvwprintw(win, 8, sz_x/2 - title.size()/2, title.c_str());
    
    for(int i = 0 ; i < sz; i++){ 
        if(i == selected) wattron(win, COLOR_PAIR(1));
        else wattron(win, COLOR_PAIR(5));
	mvwprintw(win, sz_y/2 - 1, pos[i] + form/2 - values[i].size()/2, values[i].c_str());
        mvwprintw(win, sz_y/2, pos[i], string(form, '_').c_str());
        mvwprintw(win, sz_y/2 + 1, pos[i] + form/2 - names[i].size()/2, names[i].c_str());      
        if(i == selected) wattroff(win, COLOR_PAIR(1));
	else wattroff(win, COLOR_PAIR(5));
    }
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, sz_y - 8, 8, "[A] Accept");
    mvwprintw(win, sz_y - 8, sz_x - 8 - 10, "[C] Cancel");
    wattroff(win, COLOR_PAIR(2));
    cur = pos[selected] + form/2 - values[selected].size()/2 + values[selected].size();
    wmove(win,sz_y/2 - 1, cur);
    
    
    
    wrefresh(win);    
    update_panels();
    doupdate();

    int ans = -1;
    bool acc = 0, canc = 0;
    
    while(int c = getch()){
        //printw("%d\n", c);
        

        switch(c){
            case KEY_LEFT: selected = (selected - 1 + sz) % sz; break;
            case KEY_RIGHT: selected = (selected + 1 + sz) % sz; break;

            case '0': case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': case '.': case '-':

                values[selected] += char(c);
            break;

            case KEY_BACKSPACE:
                if(!values[selected].empty()) values[selected].pop_back();
            break;

            case 'a':
                wattron(win, COLOR_PAIR(2));
                wattron(win, A_REVERSE);
                mvwprintw(win, sz_y - 8, 8, "[A] Accept");
                wattroff(win, A_REVERSE);
                mvwprintw(win, sz_y - 8, sz_x - 8 - 10, "[C] Cancel");
                wattroff(win, COLOR_PAIR(2));
                acc = 1, canc = 0;
                break;
            
            case 'c':
                wattron(win,COLOR_PAIR(2));
                wattron(win, A_REVERSE);
                mvwprintw(win, sz_y - 8, sz_x - 8 - 10, "[C] Cancel");
                wattroff(win, A_REVERSE);
                mvwprintw(win, sz_y - 8, 8, "[A] Accept");
                wattroff(win, COLOR_PAIR(2));
                acc = 0, canc = 1;
                break;

            case 10:
                if(acc){
                    ans = 1; for(int i = 0 ; i < sz ; i++) arr[i] = stof(values[i]);
                }
                else if(canc){
                    ans = 0; for(int i = 0 ; i < sz ; i++) arr[i] = 0;
                }


                break;

            default:
                wattron(win, COLOR_PAIR(2));
                mvwprintw(win, sz_y - 8, 8, "[A] Accept");
                mvwprintw(win, sz_y - 8, sz_x - 8 - 10, "[C] Cancel");
                wattroff(win, COLOR_PAIR(2));
                acc = canc = 0;
                break;

        }

        for(int i = 0 ; i < sz; i++){
            mvwprintw(win, sz_y/2 - 1, pos[i], string(form, ' ').c_str());
             
            if(i == selected) wattron(win, COLOR_PAIR(1));
            else wattron(win, COLOR_PAIR(5));
		mvwprintw(win, sz_y/2 - 1, pos[i] + form/2 - values[i].size()/2, values[i].c_str());
            mvwprintw(win, sz_y/2, pos[i], string(form, '_').c_str());
            mvwprintw(win, sz_y/2 + 1, pos[i] + form/2 - names[i].size()/2, names[i].c_str());      
            if(i == selected) wattroff(win, COLOR_PAIR(1));
		else wattroff(win, COLOR_PAIR(5));
        }
        
        cur = pos[selected] + form/2 - values[selected].size()/2 + values[selected].size();
        wmove(win,sz_y/2 - 1, cur);
        
        wrefresh(win);
        update_panels();
        doupdate();
        if(ans != -1) break;
    }

    curs_set(0);
    werase(win);
    return ans;
}

bool writeData(PANEL* pan, string title, string* names, float* arr, int sz){
    
    WINDOW* win = panel_window(pan);
    int sz_x, sz_y, form = 8, tam = form*sz + sz-1, cur;
    getmaxyx(win, sz_y, sz_x);
    
    int pos[sz];
    pos[0] = sz_x/2 - tam/2;
    for(int i = 1 ; i < sz; i++) pos[i] = pos[i-1] + 9;

    string values[sz];
    char buff[50];
    
    
    mvwprintw(win, 8, sz_x/2 - title.size()/2, title.c_str());
    for(int i = 0 ; i < sz; i++){ 
        wattron(win, COLOR_PAIR(5));
        sprintf(buff, "%.3f", arr[i]);
        values[i] = string(buff);
        mvwprintw(win, sz_y/2 - 1, pos[i] + form/2 - values[i].size()/2, values[i].c_str());
        mvwprintw(win, sz_y/2, pos[i], string(form, '_').c_str());
        mvwprintw(win, sz_y/2 + 1, pos[i] + form/2 - names[i].size()/2, names[i].c_str());    
	wattroff(win, COLOR_PAIR(5));
    }

    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, sz_y - 8,  sz_x/2 - 5, "[A] Accept");
    wattroff(win, COLOR_PAIR(2));
    
    
    
    wrefresh(win);    
    update_panels();
    doupdate();

    int ans = -1;
    bool acc = 0;
    
    while(int c = getch()){
        switch(c){
            

            case 'a':
                wattron(win, COLOR_PAIR(2));
                wattron(win, A_REVERSE);
                mvwprintw(win, sz_y - 8,  sz_x/2 - 5, "[A] Accept");
                wattroff(win, A_REVERSE);
                wattroff(win, COLOR_PAIR(2));
                acc = 1; break;

            case 10:
                if(acc){
                    ans = 0; for(int i = 0 ; i < sz ; i++) arr[i] = stof(values[i]);
                }
                break;

            default:
                wattron(win, COLOR_PAIR(2));
                mvwprintw(win, sz_y - 8, sz_x/2 - 5, "[A] Accept");
                wattroff(win, COLOR_PAIR(2));
                acc = 0;
                break;

        }
        
        wrefresh(win);
        update_panels();
        doupdate();
        if(ans != -1) break;
    }

    
    werase(win);
    return ans;
}




#endif
