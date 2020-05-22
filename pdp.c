#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>

extern Argument ss, dd, nn, r;

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
	w_write(dd.adr, ss.val);
}

void do_add() {
	w_write(dd.adr, dd.val + ss.val);
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
	w_write(dd.adr, 0);
}

Command cmd[] = {
	{0170000, 0010000, "mov", do_mov, 3},
	{0170000, 0060000, "add", do_add, 3},
	{0xFFFF , 0000000, "halt", do_halt, 0},
	{0177000, 0077000, "sob", do_sob, 12},
	{0177700, 0005000, "clr", do_clear, 1}
};