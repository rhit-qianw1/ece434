// Author: Wenzi Qian
// A C++ class to read button state using gpiod

#ifndef BUTTON_H_
#define BUTTON_H_

#include <gpiod.h>

class Button
{
public:
    Button();
    ~Button();

    // init all the stuffs. Must be called before read.
    int init(int chip_number, int line_number);
    // read button value
    int read();

private:
    int chip_num;
    unsigned int line_num;
	struct gpiod_chip *chip;
	struct gpiod_line *line;

};


#endif