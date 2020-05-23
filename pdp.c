#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>

extern Argument ss, dd, nn, r, xx;
extern int is_byte;

int N, Z, C;

void set_NZ(int w) {
	 Z = (w == 0);
     N = (is_byte ? w >> 7 : w >> 15) & 1;
}

void set_C(int w) {
	C = (is_byte ? w >> 8 : w >> 16) & 1;
}

void print_reg() {
	printf("\n");
	for (int i = 0; i < 8; ++i) {
		printf("r%d: %06o ", i, reg[i]);
	}
	printf("\n");
}

void do_halt() {
	print_reg();
	trace("THE END!!!\n");
	exit(0);
}

void do_mov() {
	set_NZ((int)ss.val);
	w_write(dd.adr, ss.val);
}

void do_add() {
	int res = ss.val + dd.val;
	set_NZ(res);
	set_C(res);
	w_write(dd.adr, (word)res);
}

void do_nothing() {
	exit(1);
}

void do_sob() {
	trace("#%06o", pc - 2 * nn.val);
	if (--reg[r.adr] != 0)
		pc -= 2 * nn.val;
}

void do_clear() {
	set_NZ(0);
	set_C(0);
	w_write(dd.adr, 0);
}

void do_movb() {
	set_NZ((int)ss.val);
	b_write(dd.adr, ss.val);
}

void do_br() {
	pc += 2 * xx.val;
	trace("%06o ", pc);
}

void do_beq() {
	if (Z)
		do_br();
}

void do_bpl() {
	if (N == 0)
		do_br();
}

void do_tst() {
	set_NZ(dd.val);
	set_C(0);
}

void do_tstb() {
	set_NZ(dd.val);
	set_C(0);
}

void do_jsr() {
	w_write(sp, reg[r.adr]);
	sp += 2;
	reg[r.adr] = pc;
	pc = dd.adr;
}

void do_rts() {
	pc = reg[r.adr];
	sp -= 2;
	w_write(r.adr, w_read(sp));
}

Command cmd[] = {
	{0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
	{0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
	{0xFFFF , 0000000, "halt", do_halt, NO_PARAMS},
	{0177000, 0077000, "sob", do_sob, HAS_NN | HAS_R},
	{0177700, 0005000, "clr", do_clear, HAS_DD},
	{0170000, 0110000, "movb", do_movb, HAS_DD | HAS_SS},
	{0xFF00, 0x0100, "br", do_br, HAS_XX},
	{0xFF00, 0x0300, "beq", do_beq, HAS_XX},
	{0xFF00, 0x8000, "bpl", do_bpl, HAS_XX},
	{0177700, 0005700, "tst", do_tst, HAS_DD},
	{0177700, 0105700, "tstb", do_tstb, HAS_DD},
	{0177000, 0004000, "jsr", do_jsr, HAS_DD | HAS_R},
	{0177770, 0000200, "rts", do_rts, HAS_RL},
	{0x0000, 0x0000, "unknown", do_nothing, NO_PARAMS}
};