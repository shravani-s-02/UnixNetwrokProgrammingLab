#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

extern int errno;
void main(){
	int fd = open("Welcome.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd == -1){
		int errsv = errno;
		fprintf(stderr, "File Welcome.txt cannot be created:%s\n",strerror(errsv));
		exit(1);
	}
		
	char *input="Hello World. Welcome to Unix System Programing";
	if(write(fd,input,strlen(input)) == -1) {
		int errsv = errno;
		fprintf(stderr, "File Welcome.txt cannot be written to:%s\n",strerror(errsv));
		exit(1);
	}
	
	if(lseek(fd,0,SEEK_SET) == -1) {
		int errsv = errno;
		fprintf(stderr, "File Welcome.txt cannot be seeked:%s\n",strerror(errsv));
		exit(1);
	}

	char output[70];
	if(read(fd,output,100) == -1) {
		int errsv = errno;
		fprintf(stderr, "File Welcome.txt cannot be read:%s\n",strerror(errsv));
	}
	else{
		printf("%s\n", output);
	}

	close(fd);
}

