#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>

extern Command cmd[];
extern word reg[8];

Argument ss, dd, nn, r, xx;

Argument get_mr(word w);
Argument get_nn(word w);
Argument get_r(word w);
Argument get_xx(word w);
Argument get_r_last(word w);

int is_byte;

void run() {
	pc = 01000;
	while(1) {
		word w = w_read(pc);
		trace("%06o %06o: ", pc, w);
		pc += 2;
		for (int i = 0; 1; ++i) {
			if ( (w & cmd[i].mask) == cmd[i].opcode) {
				trace(cmd[i].name);
				trace(" ");
				is_byte = w & 0100000;
				if (cmd[i].params & HAS_SS) {
					ss = get_mr(w >> 6);
				}
				if (cmd[i].params & HAS_DD) {
					dd = get_mr(w);
				}
				if (cmd[i].params & HAS_NN) {
					nn = get_nn(w);
				}
				if (cmd[i].params & HAS_R) {
					r = get_r(w);
				}
				if (cmd[i].params & HAS_XX) {
					xx = get_xx(w);
				}
				if (cmd[i].params & HAS_RL) {
					r = get_r_last(w);
				}
				cmd[i].do_func();
				//trace("\n");
				//print_reg();
				break;
			}
		}
		trace("\n");
	}
}

Argument get_mr(word w) {
	Argument res;
	int r = w & 7; // register
	int mode = (w >> 3) & 7;
	int x;
	switch(mode) {
		case 0:    // R3
			res.adr = r;
			res.val = reg[r];
			trace("R%o ", r);
			break;
		case 1:    // (R3)
			res.adr = reg[r];
			if (is_byte && r != 6 && r !=7)
				res.val = b_read(res.adr);
			else 
				res.val = w_read(res.adr);   // to do b_read
			trace("(R%o) ", r);
			break;
		case 2:    // (R3)+
			res.adr = reg[r];
			if (is_byte && r != 6 && r !=7) {
				res.val = b_read(res.adr);  
				reg[r] += 1;  
			}
			else {
				res.val = w_read(res.adr);   // to do b_read
				reg[r] += 2;			// to do +1
			}
			if (r == 7)
				trace("#%06o ", res.val);
			else
				trace("(R%o)+ ", r);
			break;
		case 3:   //  @(R3)+
			res.adr = reg[r];
			res.adr = w_read(res.adr);
			res.val = w_read(res.adr);
			reg[r] += 2;
			break;
		case 4:   //  -(R3)
			if (is_byte && r != 6 && r !=7) {
				reg[r] -= 1;
				res.adr = reg[r];
				res.val = b_read(res.adr);
			}
			else {
				reg[r] -= 2;
				res.adr = reg[r];
				res.val = w_read(res.adr);
			}
			if (r == 7)
				trace("#%06o ", res.val);
			else
				trace("-(R%o) ", r);
			break;
		case 6:   // x(R3)
			x = w_read(pc);
			pc += 2;
			res.adr = x + reg[r];
			res.val = w_read(res.adr);
			if (r == 7)
				trace("%06o ", res.adr);
			else
				trace("%o(R%o) ", x, r);
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

Argument get_xx(word w) {  // xx
	Argument res;
	int val;
	if ((w & 0x00FF) >> 7 == 0)
		val = w & 0x00FF;
	else
		val = (w & 0x00FF) - 0400;
	res.val = val;
	return res;
}

Argument get_r_last(word w) {  //reg
	Argument res;
	int r = w & 7; // register
	res.adr = r;
	trace("R%o ", r);
	return res;
}
 