#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define	CONSUMER	"Consumer"

int main(int argc, char **argv)
{
	int chipnumber = 1;
	struct timespec ts = { 0, 1000000 };	// 1s Timeout for event
	struct gpiod_line_event event;
	unsigned int getline_num = 14;	// GPIO Pin P8_16
	unsigned int val;
	struct gpiod_chip *chip;
	struct gpiod_line *getline;
	int i, ret;

	chip = gpiod_chip_open_by_number(chipnumber);
	if (!chip) {
		perror("Open chip failed\n");
		goto end;
	}

	getline = gpiod_chip_get_line(chip, getline_num);
	if (!getline) {
		perror("Get line failed\n");
		goto close_chip;
	}

	ret = gpiod_line_request_falling_edge_events(getline, CONSUMER);
	if (ret < 0) {
		perror("Request line as input failed\n");
		goto release_line;
	}


	/* Get */
	while(1) {
		do {
			// printf("waiting...\n");
		    ret = gpiod_line_event_wait(getline, &ts);
		} while (ret <= 0);
		
		// I'm getting a Segment failt.  event isn't correct.
		// ret = gpiod_line_event_read(getline, &event);
		// printf("ret: %d, event: %d\n", ret, event);
		// if (!ret)
		//     printf("event: %s timestamp: [%8ld.%09ld]\n",
		//     	event.event_type, event.ts.tv_sec, event.ts.tv_nsec);
		
		val = gpiod_line_get_value(getline);
		printf("%d\n", val);
		usleep(1000);
	}

release_line:
	gpiod_line_release(getline);
close_chip:
	gpiod_chip_close(chip);
end:
	return 0;
}