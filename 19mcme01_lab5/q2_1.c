#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>

int byteChecking(){
	union{
		short s;
		char c[sizeof(short)];
	}un;
	
	un.s = 0x0102;
	if(un.c[0] == 1 && un.c[1] == 2) {
		//Uses big endian byte ordering
		return 2;
	}
	
	if(un.c[0] == 2 && un.c[1] == 1) {
		//Uses little endian byte ordering
		return 1;
	}
	
	//byte ordering is unknown
	return 0;
}

uint16_t my_htons(uint16_t host16bitvalue) {
	int order;
	if((order = byteChecking()) == 0) {//If byte ordering is unknown
		printf("byte ordering is unknown. Can not convert.");
		exit;
	}
	else if(order = 1){//If byte ordering is little endian convert to big endian
		return ((host16bitvalue & 0xFF) << 8) | ((host16bitvalue >> 8) & 0xFF);
	}
	else{//If byte ordering is big endian return the same host value
		return 	host16bitvalue;
	}
}

void main(){
	uint16_t  hostShortInBytes = 0x1234;    
	printf("my_htons(0x%04x) = 0x%04x\n", hostShortInBytes, my_htons(hostShortInBytes));
}
