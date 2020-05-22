#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>

extern Command cmd[];
extern word reg[8];

Argument ss, dd, nn, r;

Argument get_mr(word w);
Argument get_nn(word w);
Argument get_r(word w);

void run() {
	pc = 01000;
	while(1) {
		word w = w_read(pc);
		trace("%06o %06o: ", pc, w);
		pc += 2;
		for (int i = 0; i <= 5; ++i) {
			if ( (w & cmd[i].mask) == cmd[i].opcode) {
				trace(cmd[i].name);
				trace(" ");
				if (cmd[i].params & HAS_DD) {
					dd = get_mr(w);
				}
				if (cmd[i].params & HAS_SS) {
					ss = get_mr(w >> 6);
				}
				if (cmd[i].params & HAS_NN) {
					nn = get_nn(w);
				}
				if (cmd[i].params & HAS_R) {
					r = get_r(w);
				}
				cmd[i].do_func();
				break;
			}
			if (i == 5) {
				trace("unknown command");
				do_nothing();
			}
		}
		trace("\n");
	}
}

Argument get_mr(word w) {
	Argument res;
	int r = w & 7; // register
	int mode = (w >> 3) & 7;
	switch(mode) {
		case 0:    // R3
			res.adr = r;
			res.val = reg[r];
			trace("R%o ", r);
			break;
		case 1:    // (R3)
			res.adr = reg[r];
			res.val = w_read(res.adr);   // to do b_read
			trace("R%o ", r);
			break;
		case 2:    // (R3)+
			res.adr = reg[r];
			res.val = w_read(res.adr);   // to do b_read
			reg[r] += 2;                 // to do +1
			if (r == 7)
				trace("#%06o ", res.val);
			else
				trace("R%o ", r);
			break;
		default:
			fprintf(stderr, 
				"Mode %o NOT IMPLEMENTED YET!\n", mode);
			exit(1);
	}
	return res;
}

Argument get_nn(word w) { // array number
	Argument res;
	int n = w & 077; // n number
	res.val = n;
	return res;
}

Argument get_r(word w) {  //reg
	Argument res;
	int r = (w >> 6) & 7; // register
	res.adr = r;
	trace("R%o ", r);
	return res;
}
 