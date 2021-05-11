// This modules derivates encoders signal to calculate motors velocity.
// The output is in RPM and the input are the encoder present counts.
module RPM(Prescaler_clk, count, RPM_Measured);
	// INPUTS AND OUTPUTS
	input					Prescaler_clk;						// 5ms clock
	input	signed		[32:0]	count;					// Present encoder count
	output	signed	[15:0]	RPM_Measured;			// Velocity in RPM
	// REGS
	reg signed			[15:0]	RPM_Measured	= 0;	// RPM velocity
	reg signed			[32:0]	last_count		= 0;	// Temporal variable for derivation.
	// SEQUENTIAL LOGIC
	always @(posedge Prescaler_clk)
		begin
			RPM_Measured	<= ((count-last_count)*3125)/10000; // Makes the derivative. Constants are calculated with encoder CPR, gearhead box and prescaler clock
			last_count		<= count; // Assign temporal variable for derivative.
		end
endmodule
