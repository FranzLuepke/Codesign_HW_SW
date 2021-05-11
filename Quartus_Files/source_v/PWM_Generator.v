// Generates PWM signal from an 8 bits number, betwenn 0 and 249.
module PWM_Generator(clock, reset, PWM_in, PWM_out);
	// INPUTS AND OUTPUTS
	input 			clock;					// Clock
	input 			reset;					// Reset
	input	[7:0]		PWM_in;					// PWM requested
	output 			PWM_out;					// PWM out signal
	// REGS
	reg				actual_output	= 0;	// Present output
	// WIRES
	wire 				posFlag;
	wire 				negFlag;
	// ASSIGNS
	assign			PWM_out			= actual_output;	// Output assign
	// MODULES
	Cycles_Counter posCounter(clock, reset,	2500,			posFlag);	// Indicates when to rise the output.
	Cycles_Counter negCounter(clock, posFlag, 10*PWM_in,	negFlag);	// Indicates when to lower the output.
	// The descent depends on the input, because it modulates the pulse width.
	// Sequential logic. STATE MACHINE
	always@(posedge clock) // State machine that translate flags into the signal.
		begin
			if(PWM_in==250)
				actual_output <= 1;
			else
				begin
					if(posFlag)
						actual_output <= 1;
					else
						begin
							if(negFlag)
								actual_output <= 0;
						end
				end
		end
endmodule
