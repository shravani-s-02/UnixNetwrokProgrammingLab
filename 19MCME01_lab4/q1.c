#include "apue.h"
#include<fcntl.h>
#include<sys/wait.h>


void main(){
	pid_t pid;
	int status, count, fd, fileNo;
	char *token, *arg[20], input[100], redirectChar[1];
	
	for(int i=0; i<20; i++) {
		arg[i] = (char*)calloc(40, sizeof(char));
	}
	
	while(1) {
		if ((pid = fork()) < 0) {
			err_sys("Fork error");
		} 
		else if (pid == 0) { //child
			printf("shell>");
			scanf("%[^\n]s", input);
			
			if(strcmp(input,"exit") == 0) {
				exit(2);
			}
			token = strtok (input, " ");
			count = 0;
			while (token != NULL) {
				if(strcmp(token,">") == 0 || strcmp(token,"<") == 0) {
					strcpy(redirectChar, token);	
					token = strtok (NULL, " ");
					
					if(strcmp(redirectChar,">") == 0) {
						if((fd = open(token, O_RDWR | O_TRUNC | O_CREAT, 0666)) < 0) {
							err_sys("open1 error");
						}
						if(dup2(fd, STDOUT_FILENO) < 0){
							err_sys("dup2,1 error");
						}
						close(fd);	
					}
					else{
						if((fd = open(token, O_RDONLY)) < 0) {
							err_sys("open2 error");
						}
						if(dup2(fd, STDIN_FILENO) < 0){
							err_sys("dup2,2 error");
						}
						close(fd);
					}
					token = strtok (NULL, " ");
				}
				else {
					strcpy(arg[count++], token);
					token = strtok (NULL, " ");	
				}
			}
					
			arg[count] = NULL;
			if(execvp(arg[0], arg) < 0) {
				err_sys("exec error");
			}
		} 
		else { //Parent
			if(wait(&status) != pid){
				err_sys("wait error");
			}
			
			if(WEXITSTATUS(status) == 2) {
				exit(0);
			}
		}
	}
}
