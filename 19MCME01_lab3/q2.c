#include "apue.h"
#include<sys/wait.h>

void main(){
	int n,status;
	pid_t pid;
	n = 5;

	for(int i=0; i<n; i++) {
		if ((pid = fork()) < 0) {
			err_sys("Fork error");
		} 
		else if (pid == 0) { //child
			exit(i);
		} 
		else { //parent
			if(wait(&status) != pid){
				err_sys("wait error");
			}
			printf("status:%d, pid:%d\n", WEXITSTATUS(status),pid);
		}	
	}
}
