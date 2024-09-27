#include "apue.h"
#include<fcntl.h>
#include<errno.h>
#define BUFFSIZE 4096

extern int errno;
int main(int argc, char *argv[])
{
	if(argc != 3) {
		printf("2 arguments required. Inuput and output file\n");
		exit(1);
	}
	
	int fd1 = open(argv[1],O_RDONLY);
	if(fd1 == -1){
		int errsv = errno;
		fprintf(stderr, "File %s cannot be opened:%s\n", argv[1], strerror(errsv));
		exit(1);
	}
	
	int fd2 = open(argv[2],O_WRONLY);
	if(fd2 == -1){
		int errsv2 = errno;
		fprintf(stderr, "File %s cannot be opened:%s\n", argv[2], strerror(errsv2));
		exit(1);
	}
	
	int	n;	
	char	buf[BUFFSIZE];

	while ((n = read(fd1, buf, BUFFSIZE)) > 0)
		if (write(fd2, buf, n) != n)
			err_sys("write error");
	
	if (n < 0)
		err_sys("read error");
	
	exit(0);
}
