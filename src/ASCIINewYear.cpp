#include <iostream>
#include <curses.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <signal.h>

struct param{
    bool draw_screen = TRUE;
    int fps = 8;
    int step = 0;
    int width, height;

    param() {getmaxyx(stdscr, height, width);}
};

void resize(param& par){
    clear();
    getmaxyx(stdscr,par.height, par.width);
    par.step = 0;
}


void clear_box(WINDOW* win, int height, int width, int y0, int x0){
    attron(COLOR_PAIR(-1));
    for (int i = 0; i < width; ++i){
        int xpos = x0 + i;
        for (int j = 0; j < height; ++j){
            int ypos = y0 + j;
            mvaddch(ypos, xpos, ' ');
        }
    }
    wrefresh(win);

    attroff(COLOR_PAIR(-1));
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
        int color_choice = (rand() % 8) + 1;
        if (draw){
            wattron(win, COLOR_PAIR(color_choice));
            mvwaddch(win, ypos, xpos, '*');
            wattroff(win, COLOR_PAIR(color_choice));
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

    init_pair(1, 0, 0);
    init_pair(2, 1, 0);
    init_pair(3, 2, 0);
    init_pair(4, 3, 0);
    init_pair(5, 4, 0);
    init_pair(6, 5, 0);
    init_pair(7, 6, 0);
    init_pair(8, 7, 0);

    use_default_colors();

    clear();

    srand(time(NULL));

}

int main(){

    init_curses();
 
    param par;

    WINDOW* win = newwin(par.height, par.width, 0, 0);
    //clear_box(win, par.height, par.width, 0,0);

    int temp_width, temp_height;
    sighandler_t signal_check = 0;
    while(par.draw_screen){
        handle_keys(par);
        firework_cone(win, par.height,par.width/2,1,par.height,
                      par.width,par.step);
        usleep(1000000/par.fps);
        getmaxyx(stdscr, temp_height, temp_width);
        if (temp_height != par.height || temp_width != par.width){
            resize(par);
        }
        ++par.step;
    }

    endwin();
}
