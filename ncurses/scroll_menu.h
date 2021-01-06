#include <stdio.h>
#include <string.h>
#include <curses.h>
using namespace std;
#ifndef SCROLL_MENU_H
#define SCROLL_MENU_H


class menu{
    public:
        string name;
        int ini, fin;
        menu(){}    
        menu(string name){ this->name = name;};
};


class scrollMenu{
    public:
        int ini, fin;
        int sz_x, sz_y, sz_menu, selected = 0;
        string full_name = "";
        string pattern = "-";
        menu* arr_menu;
        WINDOW* win;
        menu sel;
        int ind_sel;

        void draw(){
            if(sel.ini < ini) ini = sel.ini, fin = ini + sz_x;
            if(sel.fin > fin) fin = sel.fin, ini = fin - sz_x;
            
            wattron(win, COLOR_PAIR(2));
            if(ini > 0) mvwprintw(win, 0, 1, "<");
            else mvwprintw(win, 0, 1, "─");

            if(fin < full_name.length()) mvwprintw(win, 0, sz_x + 2, ">");
            else mvwprintw(win, 0, sz_x + 2, "─");
            wattroff(win, COLOR_PAIR(2));


            //wattron(win, COLOR_PAIR(2));
            //mvwprintw(win, 0, 2, full_name.substr(ini, sel.ini - ini).c_str());
            //wattroff(win, COLOR_PAIR(2));
            
            printSpecialColors(full_name.substr(ini, sel.ini - ini), 2, pattern);

            wattron(win, A_REVERSE | COLOR_PAIR(1));
            mvwprintw(win, 0, 2 + sel.ini - ini, sel.name.c_str());
            wattroff(win, A_REVERSE | COLOR_PAIR(1));
            
            printSpecialColors(full_name.substr(sel.fin, fin - sel.fin), 2 + sel.ini - ini + sel.name.length(), pattern);

            wrefresh(win);
        }

        void printSpecialColors(string s, int ini, string patt){
            int pos = 0;
            int found, found_ant = 0;

            if((found = s.find(patt)) == string::npos){
                mvwprintw(win, 0, ini, s.c_str());
                return;
            }

            while(found != string::npos){
                wattron(win, COLOR_PAIR(2));
                mvwprintw(win, 0, ini + pos, s.substr(found_ant, found - found_ant).c_str());
                wattroff(win, COLOR_PAIR(2));
                
                pos = found;
                
                wattron(win, COLOR_PAIR(1));
                mvwprintw(win, 0, ini + pos, patt.c_str());
                wattron(win, COLOR_PAIR(1));
                
                found += patt.length(), pos += 1;
                found_ant = found;
                found = s.find(patt, found);
            }
            
            
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, 0, ini + pos, s.substr(found_ant, s.length() - found_ant + 1).c_str());
            wattroff(win, COLOR_PAIR(2));

        }



        void keyHandling(int c){
            switch(c){
                case KEY_RIGHT: ind_sel++; break;
                case KEY_LEFT: ind_sel--; break;
            }
            ind_sel = (ind_sel + sz_menu) % sz_menu;
            sel = arr_menu[ind_sel];
            draw();
        }
        
        scrollMenu(WINDOW* win, menu* arr_menu, int sz_menu,  int sz_x,  int sz_y){
            this->win = win;
            this->arr_menu = arr_menu;
            this->sz_menu = sz_menu;
            this->sz_x = sz_x - 4;
            this->sz_y = sz_y;
            ini = 0, fin = this->sz_x;
            
            ind_sel = 0;
            full_name = "";
            for(int i = 0; i < sz_menu; i++){
                if(i > 0) full_name += pattern;
                arr_menu[i].ini = full_name.length();
                full_name += arr_menu[i].name; 
                arr_menu[i].fin = full_name.length();
            }
            sel = arr_menu[0];
        }

};



#endif