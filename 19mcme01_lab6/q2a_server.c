#include "unp.h"

void str_echo(int sockfd) {
	ssize_t n;
	char line[MAXLINE];
	
	for( ; ; ) {
		if((n=Readline(sockfd, line, MAXLINE)) ==  0) {
			return;
		}
		Writen(sockfd, line, n); 
	}
}

void main() {
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
	while(1) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

		if((childpid = fork()) == 0 ) {
			Close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		Close(connfd);
	}

}
