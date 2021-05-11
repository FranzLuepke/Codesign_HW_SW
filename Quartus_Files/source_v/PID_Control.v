// PID contol module for traction RPM
// Only work with whole numbers as setpoint
// Outputs configured for VNH5019 driver
module PID_Control(Prescaler_clk, Setpoint, RPM_Measured, Dir_A, Dir_B, PWM_Command);
	// INPUTS AND OUTPUTS
	input					Prescaler_clk;			// Clock input
	input signed	[15:0]	Setpoint;				// Setpoint in RPM
	input signed	[15:0]	RPM_Measured;			// Encoder measurement in RPM
	output reg		[7:0]	PWM_Command = 0;		// 8-bit PWM (from 0 to 250)
	output reg				Dir_A = 0;				// Output 1 of motor direction
	output reg				Dir_B = 0;				// Output 2 of motor direction
	// REGS
	reg signed		[31:0]	Integral_Error = 0;		// Integration variable
	// PARAMETERS
	parameter signed		K_P = 5;				// Kp*100
	parameter signed		K_I = 1;				// Ki*100
	// LOGIC
	always @(negedge Prescaler_clk)
	begin
		if((K_P*(Setpoint-RPM_Measured)+K_I*Integral_Error)/100 >= 249) // Upper saturation
			PWM_Command <= 249;
		else
			if((K_P*(Setpoint-RPM_Measured)+K_I*Integral_Error)/100 <= -249) // Lower saturation
				PWM_Command <= 249;
			else
				if(Setpoint==0) // When setpoint is zero.
					PWM_Command <= 0;
				else
					if((K_P*(Setpoint-RPM_Measured)+K_I*Integral_Error)/100 >= 0) // Without saturation, positive error. 
						PWM_Command <=  (K_P*(Setpoint-RPM_Measured)+K_I*Integral_Error)/100;
					else  // Without saturation, negative error. 
						PWM_Command <= -(K_P*(Setpoint-RPM_Measured)+K_I*Integral_Error)/100;
		if(K_P*(Setpoint-RPM_Measured)+K_I*Integral_Error > 0 & Setpoint!=0) // If error is greater than 0, motor forward.
			begin
				Dir_A<=1;
				Dir_B<=0;
			end
		else
			if(K_P*(Setpoint-RPM_Measured)+K_I*Integral_Error < 0 & Setpoint!=0) // If error is less than 0, motor backward.
				begin
					Dir_A<=0;
					Dir_B<=1;
				end
			else // Break if setpoint is zero.
				begin
					Dir_A<=1;
					Dir_B<=1;
				end
		if(Setpoint!=0) // Integral error acummulator reset if the setpoint passes by 0.
			Integral_Error <= Integral_Error + (Setpoint-RPM_Measured);
		else
			Integral_Error <= 0;
	end
endmodule
