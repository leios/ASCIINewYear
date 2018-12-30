#include <iostream>
#include <curses.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>

struct param{
    bool draw_screen = TRUE;
};

void handle_keys(param& par){
    int ch = getch();
    switch(ch){
        case 'q':
            par.draw_screen = FALSE;
    }

}

void init_curses(){
    // Initializing the screen
    initscr();

    // Disabling buffering to allow one character at a time input
    cbreak();

    // Suppress automatic echoing of typed characters
    noecho();

    // Allowing special keys
    keypad(stdscr, TRUE);
}

int main(){

    init_curses();
 
    param par;

    int width, height;
    getmaxyx(stdscr, height, width);
    WINDOW* win = newwin(24, 80, 0, 0);

    while(par.draw_screen){
        handle_keys(par);
        wrefresh(win);
    }

    endwin();
}
