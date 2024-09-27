#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

extern int errno;
int readInt(int fd) {
	ssize_t nleft;
	ssize_t nread;
	int intSize=4;
	char ptr[intSize];

	nleft = intSize;
	while (nleft > 0) {
		if((nread = read(fd, (ptr+intSize-nleft), nleft)) < 0) {
			if(errno == EINTR) {
				nread = 0;
			}
			else {
				perror("read error");
				return -1;
			}
		}
		else if (nread == 0) {
			break;
		}
		nleft -= nread;
	}
	return atoi(ptr);	
}

float readFloat(int fd) {
	ssize_t nleft;
	ssize_t nread;
	float floatValue;
	int floatSize=sizeof(floatValue);
	char ptr[floatSize];

	nleft = floatSize;
	while (nleft > 0) {
		if((nread = read(fd, (ptr+floatSize-nleft), nleft)) < 0) {
			if(errno == EINTR) {
				nread = 0;
			}
			else {
				perror("read error");
				return -1;
			}
		}
		else if (nread == 0) {
			break;
		}
		nleft -= nread;
	}
	return atof(ptr);
}

void main(int argc,char *argv[]){
	if(argc != 2) {
		printf("1 argument is required. File name\n");
		exit(1);
	}	
	
	int fd, returnVal1;
	float returnVal2;
	if((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("open error");
		exit(1);
	}
	
	if((returnVal1 = readInt(fd)) < 0 ) {
		perror("readInt error");
		exit(1);
	}
	printf("Read using readInt:%d\n", returnVal1);
	
	if(lseek(fd,SEEK_SET,0) < 0) {
		perror("lseek error");
		exit(1);
	}
	
	if((returnVal2 = readFloat(fd)) < 0 ) {
		perror("readFloat error");
		exit(1);
	}
	printf("Read using readFloat:%f\n", returnVal2);
	
	close(fd);
		
}
