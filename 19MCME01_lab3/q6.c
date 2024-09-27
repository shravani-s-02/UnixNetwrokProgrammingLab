#include "apue.h"
#include <fcntl.h>

void main(int argc, char *argv[]){
	char output[10000], input[10000];
	if(argc != 2){
		printf("One argument is required. The file path.\n");
		exit(0);
	}
	
	int fd = open(argv[1], O_RDWR | O_TRUNC);
	if(fd == -1){
		err_sys("open error");
	}
	
	//Read from STDIN and write to file
	if(read(STDIN_FILENO,input,10000) == -1) {
		err_sys("read error1");
	}
	
	if(write(fd,input,strlen(input)) == -1) {
		err_sys("write error1");
	}
	
	//Read from file and write to STDOUT
	if(lseek(fd,0,SEEK_SET) == -1) {
		err_sys("lseek error");
	}
	
	if(read(fd,output,10000) == -1) {
		err_sys("read error2");
	}
	
	if(write(STDOUT_FILENO,output,strlen(output)) == -1) {
		err_sys("write error2");
	}	
}
