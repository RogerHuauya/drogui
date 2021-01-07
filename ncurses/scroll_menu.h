#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <panel.h>
using namespace std;
#ifndef SCROLL_MENU_H
#define SCROLL_MENU_H


class menu{
    public:
        string name;
        int ini, fin, pan_x, pan_y;
        WINDOW * win;
        PANEL * pan;
        int sz_lista = 5;
        string *lista;
        int selected = 0;
        menu(){}    
        menu(string name, string* opciones, int sz_lista){
            this->sz_lista = sz_lista;
            this->lista = opciones; 
            this->name = name;
            pan_x = 20;
            pan_y = sz_lista + 2;
            win = newwin(pan_y, pan_x, 0, 0);
            wattron(win, COLOR_PAIR(2));
            box(win, 0, 0);
            wattroff(win, COLOR_PAIR(2));

            draw();
            pan = new_panel(win);
            hide_panel(pan);
            update_panels();
	        doupdate();
        };

        void draw(){
            wattron(win, COLOR_PAIR(2));
            
            for(int i = 0 ; i < sz_lista ; i++){
                if(i == selected) wattron(win, A_REVERSE | COLOR_PAIR(1));
                mvwprintw(win, 1 + i, 1, lista[i].c_str());
                if(i == selected) wattroff(win, A_REVERSE| COLOR_PAIR(1)), wattron(win, COLOR_PAIR(2));;
            }

            wattroff(win, COLOR_PAIR(2));
        }

        bool keyHandling(int c){
            bool ans = true;
            switch(c){
                case KEY_UP: 
                    if(selected == 0) ans = false;
                    else selected --; 
                    break;
                case KEY_DOWN: 
                    selected ++;
                    if(selected == sz_lista) selected --;
                    break;
            }
            draw();
            return ans;
        }
};


class scrollMenu{
    public:
        int ini, fin;
        int sz_x, sz_y, sz_menu, selected = 0, pad_x, pad_y;
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
            else mvwprintw(win, 0, 1, "-");

            if(fin < full_name.length()) mvwprintw(win, 0, sz_x + 2, ">");
            else mvwprintw(win, 0, sz_x + 2, "-");
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
                wattroff(win, COLOR_PAIR(1));
                
                found += patt.length(), pos += 1;
                found_ant = found;
                found = s.find(patt, found);
            }
            
            
            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, 0, ini + pos, s.substr(found_ant, s.length() - found_ant + 1).c_str());
            wattroff(win, COLOR_PAIR(2));

        }


        bool open = false;

        void keyHandling(int c){

            if(open){
                if(!sel.keyHandling(c)){
                    open = false;
                    hide_panel(sel.pan);
                }
            } 
            else{
                switch(c){
                    case KEY_RIGHT: if(!open) ind_sel++; break;
                    case KEY_LEFT: if(!open) ind_sel--; break;
                    case KEY_DOWN: 
                        open = true;
                        int pan_ini = (2 + sel.ini - ini);
                        if( pan_ini + sel.pan_x > sz_x ) pan_ini = (2 + sel.fin - ini - sel.pan_x );
                        move_panel(sel.pan, 1 + pad_y, pan_ini + pad_x);
                        show_panel(sel.pan); 
                        break;
                        
                    
                           
                }
                ind_sel = (ind_sel + sz_menu) % sz_menu;
                sel = arr_menu[ind_sel]; 
            }
            
            draw();
            update_panels();
            doupdate();
        }
        
        scrollMenu(WINDOW* win, menu* arr_menu, int sz_menu,  int sz_x,  int sz_y, int pad_x, int pad_y){
            this->win = win;
            this->arr_menu = arr_menu;
            this->sz_menu = sz_menu;
            this->sz_x = sz_x - 4;
            this->sz_y = sz_y;
            this->pad_x = pad_x;
            this->pad_y = pad_y;
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