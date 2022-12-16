/**********************/
/* Author: Wenzi Qian */
/* Date: 2022/12/5    */
/**********************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <termios.h>

#include "Button.h"

/* Map Objects*/

#define MAP_BLANK 0
#define MAP_X 1

/**************/

/* Global Variables */
char **g_map = NULL; // Store the map for drawing

int g_width = -1; // map width
int g_height = -1; // map height

int g_gameover = 0; // mark if the game is over

int g_cursor_x = 0; // the current x position of cursor
int g_cursor_y = 0; // the current y position of cursor

#define PEN_DOWN 0
#define PEN_UP 1
int g_pen_state = PEN_UP; // the pen state

static struct termios old, current; // in relation to get input without hit enter

Button g_btn_w; // P9_22, chip 0. line 2
Button g_btn_a; // P9_21, chip 0. line 3
Button g_btn_s; // P9_18, chip 0. line 4
Button g_btn_d; // P9_17, chip 0. line 5

/********************/

// in relation to get input without hit enter
void initTermios(int echo)
{
    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;
    if (echo)
    {
        current.c_lflag |= ECHO;
    }
    else
    {
        current.c_lflag &= ~ECHO;
    }
    tcsetattr(0, TCSANOW, &current);
}

// in relation to get input without hit enter
void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}

// get input without hit enter
char getch()
{
    char ch;
    initTermios(0);
    ch = getchar();
    resetTermios();

    return ch;
}

// fill the map with map object "map_object"
void fill_map(char map_object)
{
    for (int y = 0; y < g_height; y++)
    {
        for (int x = 0; x < g_width; x++)
        {
            g_map[y][x] = map_object;
        }
    }
}

// init necessary stuffs
void init()
{
    // init map
    printf("width: %d, height: %d\n", g_width, g_height);

    g_map = (char**)malloc(g_height * sizeof(char *));
    for (int i = 0; i < g_height; i++)
    {
        g_map[i] = (char*)malloc(g_width * sizeof(char));
    }

    fill_map(MAP_BLANK);

    // init buttons
    g_btn_w.init(0,2);
    g_btn_a.init(0,3);
    g_btn_s.init(0,4);
    g_btn_d.init(0,5);

}

// get button state and return the button pushed
char get_button(){
    
    while(true){

        int w,a,s,d;
        w = g_btn_w.read();
        a = g_btn_a.read();
        s = g_btn_s.read();
        d = g_btn_d.read();

        if(w == 0){
            if(a == 0)
            {
                usleep(1000* 1000);
                return 'p';
            }
                

            if(s == 0)
                return 'c';

            if(d == 0)
                return 'q';

            return 'w';
        }
        if(a == 0){

            return 'a';
        }
        if(s == 0){
            return 's';
        }
        if(d == 0){
            return 'd';
        }

        usleep(100000);
    }
    
    return 'n';
    
}

// handle user input
void update()
{
    switch (get_button())
    {
    case 'w':
        g_cursor_y--;
        if (g_cursor_y < 0)
            g_cursor_y = 0;
        break;
    case 'a':
        g_cursor_x--;
        if (g_cursor_x < 0)
            g_cursor_x = 0;
        break;
    case 's':
        g_cursor_y++;
        if (g_cursor_y >= g_height)
            g_cursor_y = g_height - 1;
        break;
    case 'd':
        g_cursor_x++;
        if (g_cursor_x >= g_width)
            g_cursor_x = g_width - 1;
        break;
    case 'p':
        if (g_pen_state == PEN_DOWN)
            g_pen_state = PEN_UP;
        else
            g_pen_state = PEN_DOWN;
        break;
    case 'c':
        fill_map(MAP_BLANK);
        break;
    case 'q':
        g_gameover = 1;
        break;
    }

    if (g_pen_state == PEN_DOWN)
    {
        g_map[g_cursor_y][g_cursor_x] = MAP_X;
    }
}

// render cursor
void render_cursor()
{
    if (g_pen_state == PEN_DOWN)
    {
        printf("\033[1;32mX \033[0m");
    }
    else
    {
        printf("\033[1;33mX \033[0m");
    }
}

// render map
void render_map()
{
    printf(" ");
    for (int x = 0; x < g_width; x++)
    {
        printf("--");
    }
    printf("\n");

    for (int y = 0; y < g_height; y++)
    {
        printf("| ");
        for (int x = 0; x < g_width; x++)
        {

            if (x == g_cursor_x && y == g_cursor_y)
            {
                render_cursor();
                continue;
            }

            switch (g_map[y][x])
            {
            case MAP_BLANK:
                printf("  ");
                break;

            case MAP_X:
                printf("X ");
                break;

            default:
                break;
            }
        }
        printf("|\n");
    }

    printf(" ");
    for (int x = 0; x < g_width; x++)
    {
        printf("--");
    }
    printf("\n");
}

// render status
void render_status()
{
    printf("Cursor Position: X: %d, Y:%d\n", g_cursor_x, g_cursor_y);
    if (g_pen_state == PEN_DOWN)
    {
        printf("Pen status: Down\n");
    }
    else
    {
        printf("Pen status: Up\n");
    }
}

// render all the stuff
void render()
{
    system("clear");

    render_map();

    printf("\n");
    render_status();
}

// print a text character by character with some delay in micro seconds
void print_with_delay(const char *text, unsigned int millsecs)
{
    for (int i = 0; i < strlen(text); i++)
    {
        printf("%c", text[i]);
        fflush(stdout);
        usleep(millsecs * 1000);
    }
}

int main(int argc, char *argv[])
{

    // check if the number of parameter is valid
    if (argc < 3)
    {
        printf("Please specify the width and the length of the canvas like this:\n ./game [width] [height]\n");
        return 0;
    }

    g_width = atoi(argv[1]);
    g_height = atoi(argv[2]);

    if(g_width == 0 || g_height == 0){
        printf("Invaild parameter\n");
        return 0;
    }

    // print some messages
    char welcome_string[] = "Hi! Welcome to this Etch A Sketch game!\n";
    print_with_delay(welcome_string, 40);

    char manual_string[] = "Use lower case w, a, s, d to move.\nUse lower case p to switch the pen between up and down. (Yellow \033[1;33mX\033[0m for up, and green \033[1;32mX\033[0m for down)\nUse c to clean the screen.\nUse q to quit the game\n";
    print_with_delay(manual_string, 10);

    print_with_delay("\n(Press any key to continue...)", 10);

    getch();

    system("clear");

    // init map
    init();

    // render the first frame
    render();

    while (!g_gameover)
    {
        // handle user input
        update();
        // render all stuff
        render();
    }

    for (int i = 0; i < g_height; i++)
    {
        free(g_map[i]);
    }
    free(g_map);
}