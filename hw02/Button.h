#ifndef BUTTON_H_
#define BUTTON_H_

#include <gpiod.h>

class Button
{
public:
    Button();
    ~Button();

    int init(int chip_number, int line_number);
    int read();

private:
    int chip_num;
    unsigned int line_num;
	struct gpiod_chip *chip;
	struct gpiod_line *line;

};


#endif