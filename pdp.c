#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>

extern Argument ss, dd, nn, r, xx;
extern int is_byte;

int N, Z, C;

void set_NZ(int w) {
	if (w == 0) 
		Z = 1;
	else 
		Z = 0;
	if (is_byte)
		N = (w >> 7) & 1;
	else
		N = (w >> 15) & 1;
}

void set_C(int w) {
	if (is_byte)
		C = (w >> 8) & 1;
	else
		C = (w >> 16) & 1;
}

void print_reg() {
	for (int i = 0; i < 8; ++i) {
		printf("r%d: %06o ", i, reg[i]);
	}
	printf("\n");
}

void do_halt() {
	trace("\n");
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

Command cmd[] = {
	{0170000, 0010000, "mov", do_mov, HAS_DD | HAS_SS},
	{0170000, 0060000, "add", do_add, HAS_DD | HAS_SS},
	{0xFFFF , 0000000, "halt", do_halt, NO_PARAMS},
	{0177000, 0077000, "sob", do_sob, HAS_NN | HAS_R},
	{0177700, 0005000, "clr", do_clear, HAS_DD},
	{0170000, 0110000, "movb", do_movb, HAS_DD | HAS_SS},
	{0xFF00, 0x0100, "br", do_br, HAS_XX},
	{0xFF00, 0x0300, "beq", do_beq, HAS_XX},
	{0x0000, 0x0000, "unknown", do_nothing, NO_PARAMS}
};