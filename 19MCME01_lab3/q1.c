#include "apue.h"
#include<sys/wait.h>
void main(){
	pid_t pid;
	int status;
	if ((pid = fork()) < 0) {
		err_sys("Fork error");
	} 
	else if (pid == 0) { //child
		printf("Hello from child\n");
	} 
	else { //parent
		if(wait(&status) != pid){
			err_sys("wait error");
		}
		printf("Hello from parent\n");
	}	
}
