module Cycles_Counter(clock, reset, cycles, flag);
	// INPUTS AND OUTPUTS
	input 			clock;			// Clock
	input 			reset;			// Reset
	input	[27:0]	cycles;			// Number of cycles to count
	output 			flag;				// Flag to indicate the counts ended
	// REGS
	reg	[27:0]	count = 0;		// Actual counts
	reg				out_reg = 0;	// Output
	// ASSIGNS
	assign flag = out_reg;			// Output flag assignation
	// Sequential logic
	always@(posedge clock)
		begin
			if(reset==1'b1 || count==cycles) // Resets count when the number of cycles is reached or if it doesn't have to count
				begin
					count <= 28'd0;
				end
			else // Else, counts
				count <= count + 28'd1;
		end
	always@(posedge clock)
		begin
			if(count==cycles)
				out_reg = 1;
			else
				out_reg = 0;
		end
endmodule
