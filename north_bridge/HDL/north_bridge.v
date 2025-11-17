module north_bridge
#(
	// for other bus sizes see BS8, BS16
	parameter			FSB_ADDR_WIDTH = 32,
	parameter			FSB_DATA_WIDTH = 32
)
(
	// system signals
	input wire									clk,
	input wire									nrst,

	/* FSB
	 * */
	// address and data bus
	input wire	[FSB_ADDR_WIDTH-1:2]	FSB_addr,	// addr bus truncated at bit 2 (word addressing)
	// inout wire	[FSB_DATA_WIDTH-1:0]	FSB_data,	// cant use inout in verilator (try writing a wrapper later!!!!) TODO
	input wire	[FSB_DATA_WIDTH-1:0]	FSB_data_i,
	output reg	[FSB_DATA_WIDTH-1:0]	FSB_data_o,

	input wire			[3:0]			FSB_NBE,	// byte enable
	// input output wire	[3:0]					FSB_DP,		// data parity TODO: docs
		// control signals
	// input wire									NADS,		// TODO: docs
	// output wire									NRDY,		// TODO: docs

	// bus_cycle_ctrl
	input wire							FSB_W_NR,	// write / not read
	input wire							FSB_M_NIO,	// memory / not IO
	input wire							FSB_D_NC	// data / not command
	//input wire							FSB_NLOCK,		// TODO: docs
	//input wire							FSB_NPLOCK		// TODO: docs

	// interrupts
	// output wire									INTR,		// TODO: docs
	// output wire									NMI,		// TODO: docs
	// output wire									RESET,		// TODO: docs
	// output wire									SRESET,		// TODO: docs
);
	reg [FSB_DATA_WIDTH-1:0]M_RAM[31:0];  // TEMP!!
	reg [FSB_DATA_WIDTH-1:0]IO_RAM[31:0];  // TEMP!!

	wire [2:0] bus_cycle_ctrl = {FSB_W_NR, FSB_M_NIO, FSB_D_NC};
	reg [31:0] data_mask;


	always @ (
		posedge clk,
		negedge nrst
	) begin
		if (!nrst) begin
			// reset!
		end
		else begin
			data_mask <= (
				((FSB_NBE[0] ? 32'h00 : 32'hFF) << 0)	|
				((FSB_NBE[1] ? 32'h00 : 32'hFF) << 8)	|
				((FSB_NBE[2] ? 32'h00 : 32'hFF) << 16)	|
				((FSB_NBE[3] ? 32'h00 : 32'hFF) << 24)
			);	// FSB_NBE mask
			// TODO: this happens after the next switch case?? !!!!!!!!!!!!!!!!!!!

			case (bus_cycle_ctrl)  // <= W_NR, M_NIO, D_NC
				//3'b000	:	// TODO		// READ, IO, COMMAND
				3'b001	:	FSB_data_o <= (IO_RAM[FSB_addr[6:2]] & data_mask);   // READ, IO, DATA
				//3'b010	:	// TODO		// READ, MEM, COMMAND
				3'b011	:	FSB_data_o <= (M_RAM[FSB_addr[6:2]] & data_mask); 	// READ, MEM, DATA

				//3'b100	:	// TODO		// WRITE, IO, COMMAND
				3'b101	:	IO_RAM[FSB_addr[6:2]] <= (FSB_data_i & data_mask);	 // WRITE, IO, DATA
				//3'b110	:	// TODO		// WRITE, MEM, COMMAND
				3'b111	:	M_RAM[FSB_addr[6:2]] <= (FSB_data_i);    // WRITE, MEM, DATA  // TODO!!!!!!!!!!!!!!!!

				default	:	begin /* NOP*/ end
			endcase

			// // write and read switch:
			// if (FSB_W_NR) begin
			// 	RAM[FSB_addr] <= (FSB_data_i & data_mask);
			// end else begin
			// 	FSB_data_o <= (RAM[FSB_addr] & data_mask);
			// end
		end
	end


endmodule
