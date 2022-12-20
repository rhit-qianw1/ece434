/**********************/
/* Author: Wenzi Qian */
/* Date: 2022/12/5    */
/**********************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <termios.h>

#include "Button.h"
#include "i2c-dev.h"
#include "i2cbusses.h"

/* Map Objects*/

#define MAP_BLANK 0
#define MAP_X 1

/**************/

/* Global Variables */
char **g_map = NULL; // Store the map for drawing

int g_width = 8; // map width
int g_height = 8; // map height

int g_gameover = 0; // mark if the game is over

int g_cursor_x = 0; // the current x position of cursor
int g_cursor_y = 0; // the current y position of cursor

#define PEN_DOWN 0
#define PEN_UP 1
int g_pen_state = PEN_UP; // the pen state

static struct termios old, current; // in relation to get input without hit enter

Button g_btn_w; // GPIO 31
Button g_btn_a; // GPIO 5
Button g_btn_s; // GPIO 30
Button g_btn_d; // GPIO 115
Button g_btn_pen; // GPIO 111
Button g_btn_clear; // GPIO 110

int g_i2cbus;
int g_address;
int g_file;
char g_filename[20];
int g_force = 0;

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


// check if i2c is in a good state
static int check_funcs(int file) {
        unsigned long funcs;

        /* check adapter functionality */
        if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
                fprintf(stderr, "Error: Could not get the adapter "
                        "functionality matrix: %s\n", strerror(errno));
                return -1;
        }

        if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
                fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
                return -1;
        }
        return 0;
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
    g_btn_w.init(31);
    g_btn_a.init(5);
    g_btn_s.init(30);
    g_btn_d.init(115);
    g_btn_pen.init(111);
    g_btn_clear.init(110);

    g_i2cbus = lookup_i2c_bus("2");
    printf("i2cbus = %d\n", g_i2cbus);
    if (g_i2cbus < 0)
        printf("Cannot open i2c bus 2\n");

     g_address = parse_i2c_address("0x70");
    printf("address = 0x%2x\n", g_address);
    if (g_address < 0)
        printf("Cannot open address 0x70\n");;

    g_file = open_i2c_dev(g_i2cbus, g_filename, sizeof(g_filename), 0);
//      printf("file = %d\n", file);
        if (g_file < 0
         || check_funcs(g_file)
         || set_slave_addr(g_file, g_address, g_force))
                exit(1);

        // Check the return value on these if there is trouble
        i2c_smbus_write_byte(g_file, 0x21); // Start oscillator (p10)
        i2c_smbus_write_byte(g_file, 0x81); // Disp on, blink off (p11)
        i2c_smbus_write_byte(g_file, 0xe7); // Full brightness (page 15)

}

// Writes block of data to the display
static int write_block(int file, __u16 *data) {
        int res;
        res = i2c_smbus_write_i2c_block_data(file, 0x00, 16,
                (__u8 *)data);
        return res;

}


// get button state and return the button pushed
char get_button(){
    
    while(true){

        bool w,a,s,d,pen,clear;
        w = g_btn_w.is_released();
        a = g_btn_a.is_released();
        s = g_btn_s.is_released();
        d = g_btn_d.is_released();
        pen = g_btn_pen.is_released();
        clear = g_btn_clear.is_released();

        if(w){
            return 'w';
        }
        if(a){

            return 'a';
        }
        if(s){
            return 's';
        }
        if(d){
            return 'd';
        }
        if(pen){
            return 'p';
        }
        if(clear){
            return 'c';
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

// render map
void render_map()
{
    __u16 canvas[8];
    memset(canvas, 0, 8*2);

    for (int y = 0; y < g_height; y++)
    {
        for (int x = 0; x < g_width; x++)
        {

            // render cursor
            if (x == g_cursor_x && y == g_cursor_y)
            {
                canvas[y] |= 1 << (8+x);
                continue;
            }

            switch (g_map[y][x])
            {
            case MAP_BLANK:
                
                break;

            case MAP_X:
                canvas[y] |= 1 << x;
                break;

            default:
                break;
            }
        }
    }

    write_block(g_file, canvas);

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
    // print some messages
    char welcome_string[] = "Hi! Welcome to this Etch A Sketch game!\n";
    print_with_delay(welcome_string, 40);

    print_with_delay("\n(Press any key to continue...)", 10);

    getch();

    system("clear");

    // init game
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

    
    static __u16 smile_bmp[]=
        {0x3c, 0x42, 0x2889, 0x0485, 0x0485, 0x2889, 0x42, 0x3c};

    write_block(g_file, smile_bmp);
}