#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "ioctl.h"

int main(void)
{
	int fd;
	char pwd[1024], *name, *path;
	fd=open("node",O_RDWR);
	if (fd<0) {
		perror("open");
		exit(0);
	}
	if(getcwd(pwd,sizeof(pwd)) != NULL)
		printf("create a file in %s\n",pwd);
	else {
		perror("getpwd()");
		exit(0);
	}
	name = "/kern_file";
	path=strcat(pwd,name);
	printf("%s\n",path);
	ioctl(fd,IOCTL_WRITE, path);
	close(fd);
	return 0;
}
