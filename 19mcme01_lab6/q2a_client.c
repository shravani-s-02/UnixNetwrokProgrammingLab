#include "unp.h"

void str_cli(FILE *fp, int sockfd) {
	char sendline[MAXLINE], recvline[MAXLINE];

	while(Fgets(sendline, MAXLINE, fp) != NULL) {
		Writen(sockfd, sendline, strlen(sendline));

		if(Readline(sockfd, recvline, MAXLINE) == 0 ) {
			err_quit("Server terminated prematurely");
		}
		printf("Message recieved from server:");
		Fputs(recvline, stdout);
	}
}


void main(int argc, char **argv){
	int sockfd;
	struct  sockaddr_in servaddr;

	if(argc != 2) {
		err_quit("1 argument required. The server IPaddress");
	}	

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
	str_cli(stdin, sockfd); /* do it all */
	exit(0);
}
