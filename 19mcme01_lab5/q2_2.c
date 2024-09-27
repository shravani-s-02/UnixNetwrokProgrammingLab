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

uint32_t my_htonl(uint32_t host32bitvalue) {
	int order;
	if((order = byteChecking()) == 0) {//If byte ordering is unknown
		printf("byte ordering is unknown. Can not convert.");
		exit;
	}
	else if(order = 1){//If byte ordering is little endian convert to big endian
		uint32_t bigEndian;
		uint8_t *host = (uint8_t *)&host32bitvalue;
		uint8_t *network = (uint8_t *)&bigEndian;

		memset(network, 0, sizeof(uint32_t));
 
		for (int i = 0; i < sizeof(uint32_t); i++) {
			network[sizeof(uint32_t) - 1 - i] = host[i],1;
		}

		return bigEndian;
	}
	else{//If byte ordering is big endian return the same host value
		return 	host32bitvalue;
	}
}

void main(){
	uint32_t  hostLongInBytes = 0x12345678;    
	printf("my_htonl(0x%08x) = 0x%08x\n", hostLongInBytes, my_htonl(hostLongInBytes));
}
