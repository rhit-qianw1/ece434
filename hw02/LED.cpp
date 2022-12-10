#include "LED.h"

#include <stdio.h>
#include <unistd.h>

LED::LED(){

}

LED::~LED(){

}

int LED::init(int chip_number, int line_number){
    chip = gpiod_chip_open_by_number(chip_number);
	if (!chip) {
		perror("Open chip failed\n");
		gpiod_chip_close(chip);
        return -1;
	}

	line = gpiod_chip_get_line(chip, line_number);
	if (!line) {
		perror("Get line failed\n");
        gpiod_line_release(line);
		return -1;
	}

	int ret = gpiod_line_request_output(line, "Wenzi", 0);
	if (ret < 0) {
		perror("Request line as output failed\n");
		gpiod_line_release(line);
        return -1;
	}

    return 1;
}

int LED::set(int val){
    int ret = gpiod_line_set_value(line, val);
}

