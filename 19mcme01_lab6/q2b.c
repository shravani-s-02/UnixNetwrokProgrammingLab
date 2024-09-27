#include "unp.h"

void main() {
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	ssize_t n;
	char line[MAXLINE];
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
	
		while ( (n = Readline(connfd, line, MAXLINE)) > 0)  {
   			send(connfd, line, n, 0);
  		}
		close(connfd);

 	}
	Close(listenfd);
}
