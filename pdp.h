#pragma once

typedef unsigned char byte;
typedef unsigned short int word;
typedef word Adress;

#define MEMSIZE (64 * 1024)

byte mem[MEMSIZE];

byte b_read(Adress adr);
void b_write(Adress adr, byte b);
word w_read(Adress adr);
void w_write(Adress adr, word w);
void load_file(const char* filename);
void mem_dump(Adress adr, word w);

word reg[8];
#define pc reg[7]

void do_halt();
void do_mov();
void do_add();
void do_nothing();

typedef struct {
	word mask;
	word opcode;
	char * name;
	void (*do_func)(void);
	char params;
} Command;

typedef struct {
	word val;
	word adr;
} Argument;

void run();
void trace(const char* format, ...);

#define NO_PARAMS 0
#define HAS_DD 1
#define HAS_SS 2

void print_reg();
