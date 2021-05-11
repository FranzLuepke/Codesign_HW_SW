// This module counts the edges from the quadrature encoder.
// Rotating to one side increases the count, while rotation to the opposite side decreases the count.
module Encoder(clk, reset, ChannelA, ChannelB, Count);
	// INPUTS AND OUTPUTS
	input							clk, reset, ChannelA, ChannelB;
	output signed	[31:0]	Count;
	// REGS
	reg				[2:0]		ChannelA_delayed, ChannelB_delayed;
	// Channel delays
	always @(posedge clk) ChannelA_delayed <= {ChannelA_delayed[1:0], ChannelA}; // Delayed in time encoders signal (by 1 sample)
	always @(posedge clk) ChannelB_delayed <= {ChannelB_delayed[1:0], ChannelB}; // Delayed in time encoders signal (by 1 sample)
	// WIRES
	wire Count_enable		=	ChannelA_delayed[1] ^ ChannelA_delayed[2] ^ ChannelB_delayed[1] ^ ChannelB_delayed[2]; // XOR among signals
	wire Count_direction	=	ChannelA_delayed[1] ^ ChannelB_delayed[2]; // Direction of rotation (another XOR)
	// La logica de los XOR puede ser vista en la tabla de verdad de los encoders de cuadratura
	// Delayed signals help to decrease the noise in the count.
	reg signed		[31:0]	Count = 0;
	always @(posedge clk)
		if(reset==1'b1)
			begin
				Count <= 32'b0;
			end
		else
			begin
				if(Count_enable) // Count whenever is needed depending on the rotation direction.
					begin
						if(Count_direction) 
							Count <= Count + 1;
						else 
							Count <= Count - 1;
					end
			end
endmodule
