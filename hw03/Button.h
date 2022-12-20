// Author: Wenzi Qian
// A C++ class to read button state using gpiod

#ifndef BUTTON_H_
#define BUTTON_H_

class Button
{
public:
    Button();
    ~Button();

    // init all the stuffs. Must be called before read.
    int init(int gpio);
    // read button value
    int read();
    // get if the key is down
    bool get_key_down();
    // get if a button is pressed and then released
    bool is_released();

private:
    int gpio;
    int gpio_fd;

    bool last_state;

};


#endif