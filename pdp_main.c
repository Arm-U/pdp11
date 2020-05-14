#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

void test_mem() {
    byte b0 = 0x0a;
    b_write(2, b0);
    byte bres = b_read(2);
    //printf("%02hhx = %02hhx\n", b0, bres);   // first test
    assert(b0 == bres);
    
    Adress a = 4;
    byte b1 = 0x0b;
    b0 = 0x0a;
    word w = 0x0b0a;
    b_write(a, b0);
    b_write(a + 1, b1);
    word wress = w_read(a);
    //printf("ww/br \t %04hx = %02hhx%02hhx\n", wress, b1, b0);
    assert(w == wress);
    
    a = 6;
    w = 0x0cab;
    w_write(a, w);
    wress = w_read(a);
    //printf("%04hx = %04hx\n", w, wress);
    assert(w == wress);

	a = 8;
	w = 0xff0c;
	w_write(a, w);
	b1 = b_read(a + 1);
	byte b2 = b_read(a);
	//printf("%04hx = %02hhx%02hhx\n", w, b1, b2);
	assert(((byte)(w >> 8)) == b1);
	assert(((byte)w) == b2);

	a = 6;
	b1 = 0x0b;
	b0 = 0x0a;
	w = 0x0b0a;
	b_write(a, b0);
	b_write(a + 1, b1);
	wress = w_read(a);
	//printf("ww/br \t %04hx = %02hhx%02hhx\n", wress, b1, b0);
	assert(w == wress);
}

int main(int argc, char* argv[]) {
	test_mem();
	if (argc > 1)
		load_file(argv[argc - 1]);
	else {
		printf("Usage: %s FILE\n", argv[0]);
		return 0;
	}
	run();
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
		printf("Erorr: adress should be even for read word");
		exit(1);
	}
	word w = ((word)mem[adr + 1]) << 8;
	w = w | mem[adr];
	return w;
}

void w_write(Adress adr, word w) {
	if (adr % 2) {
		printf("Erorr: adress should be even for write word");
		exit(1);
	}
	word w1 = w >> 8;
	byte b1 = ((byte)w);
	mem[adr] = b1;
	byte b2 = ((byte)w1);
	mem[adr + 1] = b2;
}

void load_file(const char* filename) {
	FILE* stream = fopen(filename, "rb");
	if (stream == NULL) {
		perror(filename);
		exit(1);
	}
	unsigned int adr, n;
	while (fscanf(stream, "%x%x", &adr, &n) == 2) {
		for (unsigned int i = 0; i < n; ++i) {
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

void trace(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
}

