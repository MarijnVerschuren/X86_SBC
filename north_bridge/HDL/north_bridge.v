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
	// TODO: final solution will use inout wire but simulation does not support this!
	input wire	[FSB_DATA_WIDTH-1:0]	FSB_data_i,
	output reg	[FSB_DATA_WIDTH-1:0]	FSB_data_o,

	input wire			[3:0]			FSB_NBE,	// byte enable
	//input output wire	[3:0]			FSB_DP,		// data parity TODO: docs
		// control signals
	input wire							FSB_NADS,	// address status / valid (active low)
	//output wire							FSB_NRDY,	// bus cycle complete signal (active low) // TODO: docs

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
	wire [31:0] data_mask = (
		((FSB_NBE[0] ? 32'h00 : 32'hFF) << 0)	|
		((FSB_NBE[1] ? 32'h00 : 32'hFF) << 8)	|
		((FSB_NBE[2] ? 32'h00 : 32'hFF) << 16)	|
		((FSB_NBE[3] ? 32'h00 : 32'hFF) << 24)
	);	// FSB_NBE mask
	//{8{FSB_NBE[0]}, 8{FSB_NBE[1]}, 8{FSB_NBE[2]}, 8{FSB_NBE[3]}};


	always @ (
		posedge clk,
		negedge nrst
	) begin
		if (!nrst) begin
			// TODO: handle reset!
		end
		else if (FSB_NADS) begin
			// TODO: handle invalid address
		end
		else begin
			case (bus_cycle_ctrl)  // <= W_NR, M_NIO, D_NC
				//3'b000	:	// TODO		// READ, IO, COMMAND
				3'b001	:	FSB_data_o <= (IO_RAM[FSB_addr[6:2]] & data_mask);		// READ, IO, DATA
				//3'b010	:	// TODO		// READ, MEM, COMMAND
				3'b011	:	FSB_data_o <= (M_RAM[FSB_addr[6:2]] & data_mask);		// READ, MEM, DATA

				//3'b100	:	// TODO		// WRITE, IO, COMMAND
				3'b101	:	IO_RAM[FSB_addr[6:2]] <= (FSB_data_i & data_mask);		// WRITE, IO, DATA
				//3'b110	:	// TODO		// WRITE, MEM, COMMAND
				3'b111	:	M_RAM[FSB_addr[6:2]] <= (FSB_data_i & data_mask);		// WRITE, MEM, DATA
				default	:	begin /* NOP*/ end
			endcase
		end
	end


endmodule

/* NOTES
 * A32–A2		M/IO D/C W/R BE3 BE2 BE1 BE0	Bus Cycle
 * 0x00000000	0    0   1   0   1   1   1		Write-back1
 * 0x00000000	0    0   1   1   1   0   1		Flush
 * 0x00000001	0    0   1   0   1   1   1		First Flush Acknowledge
 * 0x00000001	0    0   1   1   1   0   1		Second Flush Acknowledge
*/