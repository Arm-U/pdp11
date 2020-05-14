#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>

extern Command cmd[];

void run() {
	pc = 01000;
	while(1) {
		word w = w_read(pc);
		trace("%06o %06o: ", pc, w);
		pc += 2;
		if (w == 0) {
			trace("halt \n");
			do_halt();
		}
		else if ((w & 0170000) == 0010000) {
			trace("mov \n");
			do_mov();
		}
		else if ((w & 0170000) == 0060000) {
			trace("add \n");
			do_add();
		}
		else {
			trace("unknown command\n");
			do_nothing();
		}
	}
}