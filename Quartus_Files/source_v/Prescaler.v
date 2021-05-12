// Clock prescaler
module Prescaler(clock, reset, prescaler_value, out_clk);
	// INPUTS AND OUTPUTS
	input				clock;					// Clock
	input				reset;					// Reset
	input	[27:0]	prescaler_value;		// Edges to count
	output			out_clk;					// Output clock
	// REGS
	reg				actual_output = 0;	// Output
	// WIRES
	wire				flag;
	// ASSIGNS
	assign out_clk = actual_output;		// Output assignation
	// EXTERNAL MODULES
	Cycles_Counter pos_edge_counter(clock, reset, prescaler_value, flag); // Generic edges counter
	// Sequential logic - STATE MACHINE
	always@(posedge clock)
		begin
			if(flag) // Oscillation every counter edge
				actual_output<=!actual_output;
		end
endmodule
