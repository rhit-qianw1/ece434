#include <stdio.h>
#include <unistd.h>

#include "Button.h"

int main()
{
    Button btn;
    btn.init(0, 2);

    while(1) {
		printf("%d\r", btn.read());
		usleep(1000);
	}


}