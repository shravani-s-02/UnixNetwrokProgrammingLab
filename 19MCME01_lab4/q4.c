#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#define LOOP 3

static volatile sig_atomic_t sigflag; // Set nonzero by sig handler
static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int signo) {
    sigflag = 1;
}

void TELL_WAIT(void) {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        perror("signal(SIGUSR1) error");
        exit(EXIT_FAILURE);
    }
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        perror("signal(SIGUSR2) error");
        exit(EXIT_FAILURE);
    }
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    // Block SIGUSR1 and SIGUSR2, and save the current signal mask
    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        perror("SIG_BLOCK error");
        exit(EXIT_FAILURE);
    }
}

void TELL_PARENT(pid_t pid) {
    kill(pid, SIGUSR2);
}

void WAIT_PARENT(void) {
    while (sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;
}

void TELL_CHILD(pid_t pid) {
    kill(pid, SIGUSR1);
}

void WAIT_CHILD(void) {
    while (sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;
}

int main() {
	int fd, counter = 0;
	if((fd = open("counter.txt", O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0 ) {
		perror("open error");
        exit(EXIT_FAILURE);
	}
    if(write(fd, &counter, sizeof(counter)) < 0) {
		perror("wait error");
        exit(EXIT_FAILURE);
	}

    TELL_WAIT(); // Initialize synchronization

    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
   else  if (pid == 0) {// Child process
        for (int i = 0; i < LOOP; i++) {
            WAIT_PARENT();
            if(lseek(fd, 0, SEEK_SET)<0) {
				perror("lseek error");
				exit(EXIT_FAILURE);
			}
			if(read(fd, &counter, sizeof(counter)) < -1) {
				perror("read error");
				exit(EXIT_FAILURE);
			}
			printf("child is incrementing from %d\n", counter);
			counter++;
			if(lseek(fd, 0, SEEK_SET)<0) {
				perror("lseek error");
				exit(EXIT_FAILURE);
			}
			if(write(fd, &counter, sizeof(counter)) < -1) {
				perror("write error");
				exit(EXIT_FAILURE);
			}
            TELL_PARENT(getppid());
        }
    } 
	else {//Parent
        for (int i = 0; i < LOOP; i++) {
            if(lseek(fd, 0, SEEK_SET)<0) {
				perror("lseek error");
				exit(EXIT_FAILURE);
			}
			if(read(fd, &counter, sizeof(counter)) < -1) {
				perror("read error");
				exit(EXIT_FAILURE);
			}
			printf("parent is incrementing from %d\n", counter);
			counter++;
			if(lseek(fd, 0, SEEK_SET)<0) {
				perror("lseek error");
				exit(EXIT_FAILURE);
			}
			if(write(fd, &counter, sizeof(counter)) < -1) {
				perror("write error");
				exit(EXIT_FAILURE);
			}
            TELL_CHILD(pid);
            WAIT_CHILD();
        }
        wait(NULL);
    }

    close(fd);
    return 0;
}