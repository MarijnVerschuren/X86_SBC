#include "main.hpp"
#include "tests.hpp"


int main(int argc, char** argv, char** env) {
	Verilated::commandArgs(argc, argv);
	Verilated::traceEverOn(true);

	Vnorth_bridge* top = new Vnorth_bridge;	// init DUT
	VerilatedVcdC* tfp = new VerilatedVcdC;	// init trace dump

	top->trace(tfp, 99);
	tfp->open("north_bridge.vcd");

	simulate(top, tfp);

	tfp->close();
	exit(0);
}



void simulate(DUT_t* top, VerilatedVcdC* tfp) {
	// initial conditions
	top->clk = 0;
	top->nrst = 0;
	top->FSB_addr = 0x00000000UL;
	top->FSB_data_i = 0x00000000UL;
	top->FSB_NBE = 0b1111;

	top->FSB_W_NR = 0;
	top->FSB_M_NIO = 0;
	top->FSB_D_NC = 0;

		// TODO: change sim_mem_access
		// * dont do eval
		// * only set signals

		// TODO write seperate func for responce (receiving data from ram) and evaluation! (this has to be done 1 to 2 t after writing signals)

		// offset clk with signal changes!!
		// try modeling 20 Mhz and count clock cycles, then index by clk cycles and run test code only between clk cycles

	// simulation
	for (
		uint32_t c = 0, ct=0, t = 0;	/* cycle, cycle time, time */
		t < SIM_RUNTIME * FSB_CLK_PER;	/* simulate SIM_RUNTIME clock cycles*/
		t += FSB_CLK_Q_PER				/* iterate with quarter clock periods */
	) {
		if (!(t % FSB_CLK_H_PER)) {
			top->clk = !top->clk;
			c++; ct = t;
			// even c -> rising edge follows
			// odd c -> falling edge follows
		}

		// all following changes will happen exaclty in the middle of clock transitions
		if ((t - ct) == FSB_CLK_Q_PER) {
			top->nrst = (c > 10);

			test_RAM_access(top, c);
			test_IO_access(top, c);
		}

		top->eval();
		tfp->dump(t);

		if (Verilated::gotFinish()) { break; }
	}
}



void sim_FSB_access(
	Vnorth_bridge* top,
	uint32_t addr,
	uint32_t data_o,
	memory_flag_t flags,
	uint8_t nbe
) {
	top->FSB_W_NR	= (flags >> 2) & 0b1U;
	top->FSB_M_NIO	= (flags >> 1) & 0b1U;
	top->FSB_D_NC	= (flags >> 0) & 0b1U;

	top->FSB_addr	= addr;
	top->FSB_NBE	= nbe;
	top->FSB_data_i	= data_o;
}


