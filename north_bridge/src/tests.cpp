//
// Created by marijn on 11/17/25.
//
#include "tests.hpp"


void test_RAM_access(DUT_t* top, uint32_t c) {
	switch (c >> 1) {	// index on rising edge cycles
	case 10:	// write 0xDEADBEEF to memory @ 1
		sim_FSB_access(top, 1, 0xDEADBEEFUL, MEM_WRITE | MEM_DATA | MEM_RAM);
		break;

	case 11:
		sim_FSB_access(top, 1, 0, MEM_READ | MEM_DATA | MEM_RAM);
		break;

	case 12:
		if (top->FSB_data_o != 0xDEADBEEFUL) {
			//printf();
			//cprintf(RED | BOLD, "WRONG read!");
		}
		break;
	}
}

void test_IO_access(DUT_t* top, uint32_t c) {
	switch (c >> 1) {	// index on rising edge cycles
	case 15:	// write 0xFEEBA555 to IO @ 1 (BE[3] = 1)
		sim_FSB_access(top, 1, 0xFEEBA555UL, MEM_WRITE | MEM_DATA | MEM_IO, 0b0001);
		break;

	case 16:
		sim_FSB_access(top, 1, 0, MEM_READ | MEM_DATA | MEM_IO);
		break;
	}
}
// if (top->FSB_data_o != 0xFEEBA500UL) {
// 	printf(
// 		"incorrect read @ t=%d\nexpected: %X, got: %X\n\n",
// 		t, 0xFEEBA555UL, top->FSB_data_o
// 	);
// }
