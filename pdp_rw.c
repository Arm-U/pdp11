#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;         //8 bit
typedef unsigned short int word;    //16 bit
typedef word Adress;				//16bit

#define MEMSIZE (64 * 1024)

byte mem[MEMSIZE];

byte b_read  (Adress adr);
void b_write (Adress adr, byte b);
word w_read  (Adress adr);
void w_write (Adress adr, word w);

int main() {
	return 0;
}
