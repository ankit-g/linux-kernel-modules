#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ioctl.h"

#define err_chk(val, func)  do{				\
				if (val <0 ) {		\
					perror(#func);	\
					exit(0);	\
				}			\
			    }while(0)			\

enum usb_controller {
	USB_PULLUP_DISABLE,
	USB_PULLUP_ENABLE
};

int main(int argc, char **argv)
{
	int fd;
	int ret;
	enum usb_controller state = atoi(argv[1]);

	fd = open("/dev/usb_gad_node",O_RDWR);
	err_chk(fd, open);

	switch (state) {
		case USB_PULLUP_DISABLE:	
			ret = ioctl(fd, USBDSBL);
			err_chk(ret, ioctl);
			break;

		case USB_PULLUP_ENABLE:	
			ret = ioctl(fd, USBENBL);
			err_chk(ret, ioctl);
			break;
		default:
			printf("Currently not handled\n");
	}
		
	return 0;
}
