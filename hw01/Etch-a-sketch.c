#include <stdio.h>
#include <stdlib.h>

#include <termios.h>

/* Map Objects*/

#define MAP_BLANK 0
#define MAP_X 1

/**************/

/* Global Variables */
char** g_map = NULL;

int g_width = -1;
int g_height = -1;

int g_gameover = 0;

int g_cursor_x = 0;
int g_cursor_y = 0;

#define PEN_DOWN 0
#define PEN_UP
int g_pen_state = PEN_DOWN;

static struct termios old, current;

/********************/

void initTermios(int echo) 
{
  tcgetattr(0, &old);
  current = old;
  current.c_lflag &= ~ICANON;
  if (echo) {
      current.c_lflag |= ECHO;
  } else {
      current.c_lflag &= ~ECHO;
  }
  tcsetattr(0, TCSANOW, &current);
}

void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

char getch(){
    char ch;
    initTermios(0);
    ch = getchar();
    resetTermios();

    return ch;
}

void init(int width, int height){
     printf("width: %d, height: %d\n", g_width, g_height);

     g_map = malloc(g_height * sizeof(char*));
     for(int i=0; i<g_height; i++){
        g_map[i] = malloc(g_width* sizeof(char));
     }

     for(int x = 0; x < g_height; x++){
        for(int y = 0; y < g_width; y++){
            g_map[y][x] = MAP_BLANK;
        }
    }
}

void update(){
    switch(getch()){
        case 'w':
            g_cursor_y--;
            if(g_cursor_y < 0)
                g_cursor_y = 0;
            break;
        case 'a':
            g_cursor_x--;
            if(g_cursor_x < 0)
                g_cursor_x = 0;
            break;
        case 's':
            g_cursor_y++;
            if(g_cursor_y >= g_height)
                g_cursor_y = g_height - 1;
            break;
        case 'd':
            g_cursor_x++;
            if(g_cursor_x >= g_width)
                g_cursor_x = g_width - 1;
            break;
    }

    g_map[g_cursor_y][g_cursor_x] = MAP_X;
}

void render(){
    printf(" ");
    for(int x = 0; x < g_width; x++){
        printf("--");
    }
    printf("\n");

    for(int y = 0; y < g_height; y++){
        printf("| ");
        for(int x = 0; x < g_width; x++){
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
    for(int x = 0; x < g_width; x++){
        printf("--");
    }
    printf("\n");

    printf("------------------\n");
    printf("x: %d, y:%d\n", g_cursor_x, g_cursor_y);
}

int main(int argc, char* argv[]){

    printf("Hi! Welcome to this Etch A Sketch game!\n ");

    if(argc < 3){
        printf("Please specify the width and the length of the canvas like this:\n ./game [width] [height]\n");
        return 0;
    }

    g_width = atoi(argv[1]);
    g_height = atoi(argv[2]);
    
    init(g_width, g_height);

    render();
    while(!g_gameover){
        update();
        render();
    }

}