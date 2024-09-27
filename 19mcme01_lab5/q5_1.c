#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char *argv[]) {
    	if (argc != 2) {
        	printf("domain name or ip address is requried as argument\n");
        	return 1;
        }

	struct in_addr ipv4;
	struct in6_addr ipv6;
	if (inet_pton(AF_INET, argv[1], &ipv4) == 1) {
		char ipv4_str[INET_ADDRSTRLEN];
        	inet_ntop(AF_INET, &ipv4, ipv4_str, INET_ADDRSTRLEN);
        	printf("IPv4 Address in Human-readable Form: %s\n", ipv4_str);
	}
    	else if (inet_pton(AF_INET6, argv[1], &ipv6) == 1) {
        	char ipv6_str[INET6_ADDRSTRLEN];
        	inet_ntop(AF_INET6, &ipv6, ipv6_str, INET6_ADDRSTRLEN);
        	printf("IPv6 Address in Human-readable Form: %s\n", ipv6_str);
    	}
    	else {
        	struct hostent *hptr;
        	char str[INET_ADDRSTRLEN];
        
        	if ((hptr = gethostbyname(argv[1])) != NULL) {
        	   printf("Resolved IP Address: %s\n", inet_ntop(hptr->h_addrtype, argv[1],str,sizeof(str)));
 		} 
  	      else {
        	    perror("gethostbyname error");
        	    return -1;
        	}
    	}
}
