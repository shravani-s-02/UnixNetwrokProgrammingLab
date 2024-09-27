#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

extern int errno;
void main(){
	int fd = open("Hole.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(fd == -1){
		int errsv = errno;
		fprintf(stderr, "File Hole.txt cannot be created:%s\n",strerror(errsv));
		exit(1);
	}
	
	if(write(fd,"Hello",5) == -1) {
		int errsv = errno;
		fprintf(stderr, "File Hole.txt cannot be written to:%s\n",strerror(errsv));
		exit(1);
	}
	
	if(lseek(fd,1024,SEEK_CUR) == -1) {
		int errsv = errno;
		fprintf(stderr, "File Hole.txt cannot be seeked:%s\n",strerror(errsv));
		exit(1);
	}
	
	if(write(fd," World",6) == -1) {
		int errsv = errno;
		fprintf(stderr, "File Hole.txt cannot be written to:%s\n",strerror(errsv));
		exit(1);
	}
}
