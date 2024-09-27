#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

extern int errno;
void main(){
	int fd = open("Welcome.txt", O_RDWR);
	if(fd == -1){
		int errsv = errno;
		fprintf(stderr, "File Welcome.txt cannot be opened:%s\n",strerror(errsv));
		exit(1);
	}
		
	if(lseek(fd,-3,SEEK_END) == -1) {
		int errsv = errno;
		fprintf(stderr, "File Welcome.txt cannot be seeked:%s\n",strerror(errsv));
		exit(1);
	}

	if(write(fd,"ming",4) == -1) {
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
	if(read(fd,output,70) == -1) {
		int errsv = errno;
		fprintf(stderr, "File Welcome.txt cannot be read:%s\n",strerror(errsv));
	}
	else{
		printf("%s\n", output);
	}

	close(fd);
}

