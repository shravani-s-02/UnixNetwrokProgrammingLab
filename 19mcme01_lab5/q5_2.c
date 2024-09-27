#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>

//sock_ntop_host function found on github https://github.com/hailinzeng/Unix-Network-Programming/blob/master/lib/sock_ntop_host.c
char *sock_ntop_host(const struct sockaddr *sa, socklen_t salen) {
    static char str[128];		/* Unix domain is largest */

	switch (sa->sa_family) {
	case AF_INET: {
		struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

		if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return(NULL);
		return(str);
	}

#ifdef	IPV6
	case AF_INET6: {
		struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

		if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
			return(NULL);
		return(str);
	}
#endif

#ifdef	AF_UNIX
	case AF_UNIX: {
		struct sockaddr_un	*unp = (struct sockaddr_un *) sa;

			/* OK to have no pathname bound to the socket: happens on
			   every connect() unless client calls bind() first. */
		if (unp->sun_path[0] == 0)
			strcpy(str, "(no pathname bound)");
		else
			snprintf(str, sizeof(str), "%s", unp->sun_path);
		return(str);
	}
#endif

#ifdef	HAVE_SOCKADDR_DL_STRUCT
	case AF_LINK: {
		struct sockaddr_dl	*sdl = (struct sockaddr_dl *) sa;

		if (sdl->sdl_nlen > 0)
			snprintf(str, sizeof(str), "%*s",
					 sdl->sdl_nlen, &sdl->sdl_data[0]);
		else
			snprintf(str, sizeof(str), "AF_LINK, index=%d", sdl->sdl_index);
		return(str);
	}
#endif
	default:
		snprintf(str, sizeof(str), "sock_ntop_host: unknown AF_xxx: %d, len %d",
				 sa->sa_family, salen);
		return(str);
	}
    return (NULL);
}

int main(int argc, char *argv[]) {
    	printf("1");
	if (argc != 2) {
        	printf("domain name is requried as argument\n");
        	exit(1);
        }
	
	struct addrinfo hint, *result, *iter;
	memset(&hint,0,sizeof(struct addrinfo));
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = 0;
	//int res;
	if(getaddrinfo(argv[1],NULL,&hint,&result) != 0) {
		//printf("res:%d", res);
		printf("getaddrinfo error");
		exit(1);
	}

	static char *str;
	iter = result;
	while(iter != NULL) {
		printf("%s\n", sock_ntop_host(iter->ai_addr, iter->ai_addrlen));
		iter = iter->ai_next;	
	}
	freeaddrinfo(result);	
	return 0;
}
