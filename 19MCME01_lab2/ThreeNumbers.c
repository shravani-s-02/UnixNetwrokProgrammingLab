#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>

void main(int argc, char* argv[]){
	int num1, num2, num3;
	printf("Enter integer1:");								
	scanf("%d", &num1);
	
	printf("Enter integer2:");								
	scanf("%d", &num2);
	
	printf("Enter integer3:");								
	scanf("%d", &num3);
	
	int fd = open("threeNum.txt", O_RDWR | O_APPEND | O_TRUNC);
	if(fd == -1){
		int errsv = errno;
		fprintf(stderr, "File threeNum.txt cannot be opened:%s\n",strerror(errsv));
		exit(1);
	}	
	
	//Write
	char number[10];
	sprintf(number,"%d",num1);
	if(write(fd,number,strlen(number)) == -1) {
		int errsv = errno;
		fprintf(stderr, "File threeNum.txt cannot be written to:%s\n",strerror(errsv));
		exit(1);
	}
	
	if(write(fd," ",1) == -1) {
		int errsv = errno;
		fprintf(stderr, "File threeNum.txt cannot be written to:%s\n",strerror(errsv));
		exit(1);
	}
	
	sprintf(number,"%d",num2);
	if(write(fd,number,strlen(number)) == -1) {
		int errsv = errno;
		fprintf(stderr, "File threeNum.txt cannot be written to:%s\n",strerror(errsv));
		exit(1);
	}
	
	if(write(fd," ",1) == -1) {
		int errsv = errno;
		fprintf(stderr, "File threeNum.txt cannot be written to:%s\n",strerror(errsv));
		exit(1);
	}
	
	sprintf(number,"%d",num3);
	if(write(fd,number,strlen(number)) == -1) {
		int errsv = errno;
		fprintf(stderr, "File threeNum.txt cannot be written to:%s\n",strerror(errsv));
		exit(1);
	}
	
	//Set offset to beginning 
	if(lseek(fd,0,SEEK_SET) == -1) {
		int errsv4 = errno;
		fprintf(stderr, "File threeNum.txt cannot be seeked:%s\n",strerror(errsv4));
		exit(1);
	}
	
	//Read
	char output[70];
	if(read(fd,output,70) == -1) {
		int errsv5 = errno;
		fprintf(stderr, "File threeNum.txt cannot be read:%s\n",strerror(errsv5));
	}
	else{
		printf("%s\n", output);
	}
	close(fd);							
}
