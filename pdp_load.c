#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned char byte;         //8 bit
typedef unsigned short int word;    //16 bit
typedef word Adress;				//16bit

#define MEMSIZE (64 * 1024)

byte mem[MEMSIZE];

byte b_read(Adress adr);
void b_write(Adress adr, byte b);
word w_read(Adress adr);
void w_write(Adress adr, word w);
//void load_file();
void load_file(const char* filename);
void mem_dump(Adress adr, word w);

void test_mem() {
    /*byte b0 = 0x0a;
    b_write(2, b0);
    byte bres = b_read(2);
    printf("%02hhx = %02hhx\n", b0, bres);   // first test
    assert(b0 == bres);*/
    
    /*Adress a = 4;
    byte b1 = 0x0b;
    byte b0 = 0x0a;
    word w = 0x0b0a;
    b_write(a, b0);
    b_write(a + 1, b1);
    word wress = w_read(a);
    printf("ww/br \t %04hx = %02hhx%02hhx\n", wress, b1, b0);
    assert(w == wress);*/
    
    /*Adress a = 6;
    word w = 0x0cab;
    w_write(a, w);
    word wress = w_read(a);
    printf("%04hx = %04hx\n", w, wress);
    assert(w == wress);*/

	/*Adress a = 8;
	word w = 0xff0c;
	w_write(a, w);
	byte b1 = b_read(a + 1);
	byte b2 = b_read(a);
	printf("%04hx = %02hhx%02hhx\n", w, b1, b2);
	assert(((byte)(w >> 8)) == b1);
	assert(((byte)w) == b2);*/

	Adress a = 6;
	byte b1 = 0x0b;
	byte b0 = 0x0a;
	word w = 0x0b0a;
	b_write(a, b0);
	b_write(a + 1, b1);
	word wress = w_read(a);
	printf("ww/br \t %04hx = %02hhx%02hhx\n", wress, b1, b0);
	printf("%06o = %03o%03o\n", wress, b1, b0);
	assert(w == wress);
}

void test_load(const char* filename) {
	load_file(filename);
	mem_dump(0x40, 30);
}

int main(int argc, char* argv[]) {
	test_mem();
	if (argc > 1)
		test_load(argv[argc - 1]);
	else {
		perror("add file to read for start program");
		return 0;
	}
	return 0;
}

byte b_read(Adress adr) {
	return mem[adr];
}

void b_write(Adress adr, byte b) {
	mem[adr] = b;
}

word w_read(Adress adr) {
	if (adr % 2) {
		perror("Adress should be even for read word");
		exit(1);
	}
	word w = ((word)mem[adr + 1]) << 8;
	w = w | mem[adr];
	//printf("w = %04x\n", w);
	return w;
}

void w_write(Adress adr, word w) {
	if (adr % 2) {
		perror("Adress should be even for write word");
		exit(1);
	}
	word w1 = w >> 8;
	byte b1 = ((byte)w);
	mem[adr] = b1;
	byte b2 = ((byte)w1);
	mem[adr + 1] = b2;
	//printf("b1 = %02hhx, b2 = %02hhx\n", b1, b2);
}

/*void load_file() {
	int adr, n;
	while(0 < scanf("%x%x", &adr, &n)) {
		for (int i = 0; i < n; ++i) {
			int _byte;
			scanf("%x", &_byte);
			b_write(adr, _byte);
			++adr;
		}
	}
}*/

void load_file(const char* filename) {
	FILE* stream = fopen(filename, "r");
	if (stream == NULL) {
		perror("Wrong file");
		exit(1);
	}
	while (!feof(stream)) {
		unsigned int adr, n;
		fscanf(stream, "%x%x", &adr, &n);
		for (int i = 0; i < n; ++i) {
			unsigned int _byte;
			fscanf(stream, "%x", &_byte);
			b_write(adr, _byte);
			++adr;
		}
	}
	fclose(stream);
}

void mem_dump(Adress start, word n) {
	for (int i = 0; i < n / 2; ++i) {
		printf("%06o : ", start);
		word w = w_read(start);
		printf("%06o\n", w);
		start += 2;
	}
	if (n % 2)
		mem_dump(start, 2);
}