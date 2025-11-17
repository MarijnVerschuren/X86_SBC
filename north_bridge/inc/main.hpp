//
// Created by marijn on 11/17/25.
//
#ifndef MAIN_HPP
#define MAIN_HPP

#include "Vnorth_bridge.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include <stdint.h>
#include <stdio.h>

//#include "cprintf.h"


// timescale: ns
// #define FSB_FREQ		24*10e6							/* simulated clock speed	*/
// #define SIM_RUNTIME		200								/* in clock cycles			*/
// #define FSB_CLK_PER		((uint32_t)((10e9)/FSB_FREQ))	/* clock cycle duration		*/
// #define FSB_CLK_H_PER	(FSB_CLK_PER >> 1)				/* clock edge duration		*/
// #define FSB_CLK_Q_PER	(FSB_CLK_PER >> 2)				/* half clock edge duration	*/

// cude time devision TODO: change dump scheme and use above defines
#define SIM_RUNTIME		200	/* in clock cycles			*/
#define FSB_CLK_PER		4	/* clock cycle duration		*/
#define FSB_CLK_H_PER	2	/* clock edge duration		*/
#define FSB_CLK_Q_PER	1	/* half clock edge duration	*/


typedef Vnorth_bridge DUT_t;


typedef enum {  // TODO improve names
	MEM_WRITE	= 0b100U,
	MEM_READ	= 0b000U,

	MEM_RAM		= 0b010U,
	MEM_IO		= 0b000U,

	MEM_DATA	= 0b001U,
	MEM_CMD		= 0b000U,
} memory_flag_t;
__always_inline memory_flag_t operator|(memory_flag_t a, memory_flag_t b) { return (memory_flag_t)(((uint8_t)a) | ((uint8_t)b)); }


void simulate(Vnorth_bridge* top, VerilatedVcdC* tfp);

void sim_FSB_access(
	Vnorth_bridge* top,
	uint32_t addr,
	uint32_t data_o,
	memory_flag_t flags,
	uint8_t nbe = 0b0000
);


#endif //MAIN_HPP
