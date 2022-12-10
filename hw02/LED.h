#ifndef LED_H_
#define LED_H_

#include <gpiod.h>

class LED
{
public:
    LED();
    ~LED();

    int init(int chip_number, int line_number);
    int set(int val);

private:
    int chip_num;
    unsigned int line_num;
	struct gpiod_chip *chip;
	struct gpiod_line *line;

};


#endif