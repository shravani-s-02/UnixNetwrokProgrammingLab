#include "apue.h"
#include<sys/wait.h>

void main(){
	pid_t pid;
	int status, count;
	char *token, *arg[20], input[100];
	
	while(1) {
		if ((pid = fork()) < 0) {
			err_sys("Fork error");
		} 
		else if (pid == 0) { //child
			printf("shell>");
			scanf("%[^\n]s", input);
			
			if(strcmp(input,"exit") == 0) {
				exit(1);
			}
			token = strtok (input, " ");
			count = 0;
			while (token != NULL) {
				arg[count++] = token;
				token = strtok (NULL, " ");
			}
			
			if(execvp(arg[0],arg) < 0) {
				err_sys("Exec error");
			}
		} 
		else {
			if(wait(&status) != pid){
				err_sys("wait error");
			}
			
			if(WEXITSTATUS(status) == 1) {
				exit(0);
			}
		}
	}
}
