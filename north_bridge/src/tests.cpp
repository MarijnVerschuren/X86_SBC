//
// Created by marijn on 11/17/25.
//
#include "tests.hpp"



void check_result(uint32_t data, uint32_t expected, uint32_t c) {
	bool read = data == expected;
	cprintf(
		(read ? GREEN : RED) | BOLD,
		"%s read from memory @ %d\naddress: %d, expected: 0x%08X, got: 0x%08X\n\n",
		(read ? "Correct" : "Incorrect"), c, 1, expected, data
	);
}



void test_RAM_access(DUT_t* top, uint32_t c) {
	switch (c) {	// index on rising edge cycles
	case 10:	// write 0xDEADBEEF to memory @ 1
		sim_FSB_access(top, 1, 0xDEADBEEFUL, MEM_WRITE | MEM_DATA | MEM_RAM);
		break;

	case 11:
		sim_FSB_access(top, 1, 0, MEM_READ | MEM_DATA | MEM_RAM);
		break;

	case 12:
		check_result(top->FSB_data_o, 0xDEADBEEFUL, c);
		break;
	}
}

void test_IO_access(DUT_t* top, uint32_t c) {
	switch (c) {	// index on rising edge cycles
	case 15:	// write 0xFEEBA555 to IO @ 1 (BE[3] = 1)
		sim_FSB_access(top, 1, 0xFEEBA555UL, MEM_WRITE | MEM_DATA | MEM_IO, 0b0001);
		break;

	case 16:
		sim_FSB_access(top, 1, 0, MEM_READ | MEM_DATA | MEM_IO);
		break;

	case 17:
		check_result(top->FSB_data_o, 0xFEEBA500UL, c);
		break;
	}
}
