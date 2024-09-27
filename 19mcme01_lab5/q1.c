#include <stdio.h>
#include <stdlib.h>

void byteChecking(){
	union{
		short s;
		char c[sizeof(short)];
	}un;
	
	un.s = 0x0102;
	if(un.c[0] == 1 && un.c[1] == 2) {
		printf("Uses big endian byte ordering\n");
	}
	else if(un.c[0] == 2 && un.c[1] == 1) {
		printf("Uses little endian byte ordering\n");
	}
	else{
		printf("byte ordering is unknown");
	}
}

void main(){
	byteChecking();
}
