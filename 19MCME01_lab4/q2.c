#include "apue.h"
#include <sys/wait.h>
#include <fcntl.h>

#define INPUT_SIZE 100

void execute(char *command) {
    char *arg[INPUT_SIZE], *token = strtok(command, " ");
    int count = 0;

    while (token != NULL && count < INPUT_SIZE - 1) {
        arg[count++] = token;
        token = strtok(NULL, " ");
    }
    
    arg[count] = NULL;
	if(execvp(arg[0], arg) < 0) {
		err_sys("exec error");
	}
}

void main (){
    pid_t pid;

    char input[INPUT_SIZE],*token, *inRedirect, *outRedirect , *file1 = NULL, *file2 = NULL, *commands[INPUT_SIZE];
    int status, count, fd1, fd2;
    
    while (1) {
        printf("shell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            err_sys("fgets error");
        }

        input[strcspn(input, "\n")] = '\0';  // Remove trailing newline

        if (strcmp(input, "exit") == 0) {
            exit(2);
        }

        file1 = NULL;
        file2 = NULL;

        inRedirect = strstr(input, "<");
        outRedirect = strstr(input, ">");

        if (inRedirect) {
            *inRedirect = '\0';
            file1 = strtok(inRedirect + 1, " ");
            file1 = strtok(file1, ">");
            file1 = strtok(file1, "<");
        }

        if (outRedirect) {
            *outRedirect = '\0';
            file2 = strtok(outRedirect + 1, " ");
            file2 = strtok(file2, ">");
            file2 = strtok(file2, "<");
        }
        count = 0;
        
        token = strtok(input, "|");
        while (token != NULL && count < INPUT_SIZE) {
            commands[count++] = token;
            token = strtok(NULL, "|");
        }

        int pipefds[2] = {-1, -1};
        int pipefdsPast[2] = {-1, -1};
        for (int i = 0; i < count; i++) {
            if (i < count - 1) {
                if (pipe(pipefds) < 0) {
                    err_sys("Pipe error");
                }
            }

            if ((pid = fork()) < 0) {
                err_sys("Fork error");
            } 
            else if (pid == 0) {// Child
                
                // Input redirection
                if (file1) {
                    if ((fd1 = open(file1, O_RDONLY)) < 0) {
                        err_sys("Input redirection failed");
                    }
                    if(dup2(fd1, STDIN_FILENO) < 0){
						err_sys("dup2 error");
					}
                    close(fd1);
                } 
                else if (i > 0) {
                    if(dup2(pipefdsPast[0], STDIN_FILENO) < 0){
						err_sys("dup2 error");
					}
                }

                // Output redirection
                if (file2) {
                    if ((fd2= open(file2, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
                        err_sys("Output redirection failed");
                    }
                    if(dup2(fd2, STDOUT_FILENO) < 0){
						err_sys("dup2 error");
					}
                    close(fd2);
                } 
                else if (i < count - 1) {
                    if(dup2(pipefds[1], STDOUT_FILENO) < 0){
						err_sys("dup2 error");
					}
                }

                // Close pipes
                if (pipefdsPast[0] != -1) {
                    close(pipefdsPast[0]);
                    close(pipefdsPast[1]);
                }

                if (pipefds[0] != -1) {
                    close(pipefds[0]);
                    close(pipefds[1]);
                }

                execute(commands[i]);
            } 
            else { // Parent
                if (pipefdsPast[0] != -1) {
                    close(pipefdsPast[0]);
                    close(pipefdsPast[1]);
                }
                pipefdsPast[0] = pipefds[0];
                pipefdsPast[1] = pipefds[1];

                if(wait(&status) != pid){
				    err_sys("wait error");
			    }
			
			    if(WEXITSTATUS(status) == 2) {
				    exit(0);
			    }
            }
        }
        if (pipefdsPast[0] != -1) {
            close(pipefdsPast[0]);
            close(pipefdsPast[1]);
        }
        while (wait(NULL) > 0);
    }
}