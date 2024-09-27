#include "apue.h"
#include<sys/wait.h>
void main(){
	pid_t pid;
	int status;
	if ((pid = fork()) < 0) {
		err_sys("Fork error");
	} 
	else if (pid == 0) { //child
		exit(0);
	} 
	else { //parent
		sleep(5);
	}	
	
	if((status = system("ps -al")) < 0 ) {
		err_sys("system() error");
	}	
}
