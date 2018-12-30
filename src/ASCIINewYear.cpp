#include <iostream>
#include <curses.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <random>

struct param{
    bool draw_screen = TRUE;
};

void clear_box(WINDOW* win, int height, int width, int y0, int x0, int color){
    init_pair(1, color, color);
    attron(COLOR_PAIR(1));
    for (int i = 0; i < width; ++i){
        int xpos = x0 + i;
        for (int j = 0; j < height; ++j){
            int ypos = y0 + j;
            mvaddch(ypos, xpos, ' ');
        }
    }
    wrefresh(win);

    //attroff(COLOR_PAIR(1));
}

// width will be constrained to an odd number
void firework_cone(WINDOW* win, int y0, int x0, int priority, int height,
                   int width, int timestep){
    if (timestep > height){
        return;
    }

    if (width % 2 != 0){
        width += 1;
    }

    // Define the spaces we can draw on
    int ypos = y0 - timestep;

    double cone_width = (double)(width*0.5)/(height)*2.0*timestep;
    for (int i = 0; i < cone_width; ++i){
        int xpos = x0 - (cone_width/2) + i;
        bool draw = (rand() % 2) == 1;
        if (draw){
            mvwaddch(win, ypos, xpos, '*');
        }
    }

    wrefresh(win);
}

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

    // Allows for dynamic getch()
    nodelay(stdscr, TRUE);

    // Starting color values
    start_color();

    clear();

    srand(time(NULL));

}

int main(){

    init_curses();
 
    param par;

    int width, height, fps;
    fps = 8;
    getmaxyx(stdscr, height, width);
    WINDOW* win = newwin(height, width, 0, 0);
    clear_box(win, height, width, 0,0,COLOR_BLACK);

    int i = 0;
    int xloc = 0;
    while(par.draw_screen){
        handle_keys(par);
        firework_cone(win, height,width/2,1,height,width,i);
        usleep(1000000/fps);
        ++i;
    }

    endwin();
}
