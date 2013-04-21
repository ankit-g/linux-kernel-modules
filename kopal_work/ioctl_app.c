#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include "ioctl.h"

int main(void)
{
	int fd;
	fd=open("node",O_RDWR);
	if (fd<0) {
		perror("open");
		exit(0);
	}
	
	ioctl(fd,IOCTL_HELLO);
	close(fd);
	return 0;
}
