#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

extern int errno;
void main(int argc, char *argv[]){
	if(argc != 2) {
		printf("1 argument is required. File path\n");
		exit(1);
	}
		
	int fd = open(argv[1],O_WRONLY);
	if(fd == -1){
		int errsv = errno;
		fprintf(stderr, "File %s cannot be opened:%s\n", argv[1], strerror(errsv));
		exit(1);
	}
	
	int fd2 = dup2(fd, 1);
	if(fd2 == -1){
		int errsv = errno;
		fprintf(stderr, "File descriptor could not be duped:%s\n", strerror(errsv));
		exit(1);
	}
	
	printf("Hello World");
	close(fd);
}
