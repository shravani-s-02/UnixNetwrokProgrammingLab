#include "apue.h"

void handler(int signo) {
	while(waitpid( -1, NULL, WNOHANG) > 0);
}

void main(){
	pid_t pid;
	int status;
	
	signal(SIGCHLD, handler);
	if ((pid = fork()) < 0) {
		err_sys("Fork error");
	} 
	else if (pid == 0) { //child
		printf("Hello from child\n");
	} 
	else { //parent
		printf("Hello from parent\n");
		sleep(5);
		system("ps -al");
	}
}
