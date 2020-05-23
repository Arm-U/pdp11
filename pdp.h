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
#define sp reg[6]

void do_halt();
void do_mov();
void do_add();
void do_nothing();
void do_sub();
void do_clear();
void do_clear();
void do_movb();
void do_br();
void do_beq();
void do_bpl();
void do_tst();
void do_tstb();
void do_jsr();
void do_rts();

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
#define HAS_NN 4
#define HAS_R 8
#define HAS_XX 16
#define HAS_RL 32  // when register last 3 bytes

void print_reg();

void set_NZ();
void set_C();

#define ostat 0177564  // display status register
#define odata 0177566  // display data register