#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

#include "Button.h"
#include "gpio-utils.h"

Button::Button(){
	last_state = true;
}

Button::~Button(){

}

int Button::init(int gpio){
	this->gpio = gpio;
    gpio_export(gpio);
	gpio_set_dir(gpio, "in");

    return 1;
}

int Button::read(){
	unsigned int value;
	gpio_get_value(this->gpio, &value);

    return value;
}

bool Button::get_key_down() {
	return read() == 0;
}

bool Button::is_released() {

	bool current_state = get_key_down();
	if (current_state != last_state) {
		last_state = current_state;
		if (last_state) {
			return true;
		}
	}

	return false;

}