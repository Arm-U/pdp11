#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>

void do_halt() {
	printf("THE END1!!!\n");
	exit(0);
}

void do_mov() {}

void do_add() {}

void do_nothing() {}

Command cmd[] = {
	{0170000, 0010000, "mov", do_mov},
	{0170000, 0060000, "add", do_add}
};