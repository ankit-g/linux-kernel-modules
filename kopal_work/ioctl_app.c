#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ioctl.h"

int main(int argc,char *argv[])
{
	int fd;
	char pwd[1024], *name, *path;
	fd=open("node",O_RDWR);
	if (fd<0) {
		perror("open");
		exit(0);
	}
	if(getcwd(pwd,sizeof(pwd)) == NULL) {	/* getcwd() returns the current working directory path in the pointer passed */
		
		perror("getpwd()");
		exit(0);
	}
	if(argc<2)
		printf("Invalid arguments\n");
	else
		name=argv[1]; 			/* receives file name from command line */
		
	printf("create a file named '%s' at path '%s' using ioctl\n",name,pwd);
	path=strcat(pwd,name);
	ioctl(fd,IOCTL_WRITE, path);
	close(fd);
	return 0;
}
