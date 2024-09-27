#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

extern int errno;

int readInt(int fd) {
	ssize_t nleft;
	ssize_t nread;
	int intSize=4;
	char ptr[intSize];

	nleft = intSize;
	while (nleft > 0) {
		if((nread = read(fd, ptr+intSize-nleft, nleft)) < 0) {
			if(errno == EINTR) {
				nread = 0;
			}
			else {
				perror("read error");
				return -1;
			}
		}
		else if (nread == 0) {
			break;
		}
		nleft -= nread;
	}
	return *(int*)ptr;	
}

int writeInt(int fd, int num) {
	ssize_t nleft;
	ssize_t nwritten; 
	
	char *ptr = (char*)&num;
	int intSize=4;
	nleft = intSize;
	while (nleft > 0) {
		if((nwritten = write(fd, ptr+intSize-nleft, nleft)) <= 0) {
			if(nwritten < 0 && errno == EINTR) {
				nwritten = 0;
			}
			else {
				perror("read error");
				return -1;
			}
		}
		nleft -= nwritten;
	}	
	return intSize;
}


void serialization(int fd, int array[], int n ) {
	if(writeInt(fd,n) < 0) {
		perror("writeInt error");
		exit(1);
	}
	
	for(int i = 0; i < n; i++) {
		if(writeInt(fd, array[i]) < 0) {
			perror("writeInt error");
			exit(1);
		}
	}
}


int *deSerialization(int fd) {
	int size, value;
	if((size = readInt(fd)) < 0) {
		perror("readInt error");
		exit(1);
	}
		
	int *array = (int*) malloc(size * sizeof(int));
	for(int i = 0; i < size; i++) {
		if((value = readInt(fd)) < 0) {
			perror("readInt error");
			exit(1);
		}
		array[i] = value;
	}
	return array;
} 
 
void main(){
	int fd;
	if((fd = open("fileQ4.txt", O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
		perror("open error");
		exit(1);
	}
	
	printf("Serialized array is:\n");
	int array1[10];
	for(int i = 0; i<10; i++) {
		array1[i] = rand()%19;
		printf("%d\n", array1[i]);
	}	
	
	serialization(fd,array1,sizeof(array1)/sizeof(array1[0]));
	
	if(lseek(fd,SEEK_SET,0) < 0) {
		perror("lseek error");
		exit(1);
	}
	
	int *array2;
	array2 = deSerialization(fd);
	printf("Deserialized array is:\n");
	for(int i = 0; i<10; i++) { 
		printf("%d\n", array2[i]);
	}
	
	close(fd);
		
}
