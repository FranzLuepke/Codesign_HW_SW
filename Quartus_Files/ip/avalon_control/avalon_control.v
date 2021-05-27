// avalon_control.v
`timescale 1 ps / 1 ps
module avalon_control (
		input  wire	[7:0]		avs_s0_address,     //	avs_s0.address
		input  wire				avs_s0_read,        //	.read
		output wire	[31:0]	avs_s0_readdata,    //	.readdata
		input  wire				avs_s0_write,       //	.write
		input  wire	[31:0]	avs_s0_writedata,   //	.writedata
		output wire				avs_s0_waitrequest, //	.waitrequest
		input  wire				clock_clk,          //	clock.clk
		input  wire				reset_reset,        //	reset.reset
		output reg	[15:0]	RPM,                //	RPM.new_signal
		input  wire	[15:0]	PWM,                //	PWM.new_signal
		output reg	[15:0]	KP,                 //	PID.new_signal
		output reg	[15:0]	KI,                 //	.new_signal_1
		output reg	[15:0]	KD,                 //	.new_signal_2
		input  wire				DIR                 //	DIR.new_signal
	);
	// ASSIGNS
	assign						avs_s0_waitrequest	= 1'b0;
	// COMBINATIONAL LOGIC
	// Read
	always@(avs_s0_read)
		begin
			if(avs_s0_read)
				begin
					case(avs_s0_address)
						8'd0		:	avs_s0_readdata	=	{16'b0,PWM};
						8'd1		:	avs_s0_readdata	=	{31'b0,DIR};
						8'd2		:	avs_s0_readdata	=	{16'b0,RPM};
						8'd3		:	avs_s0_readdata	=	{16'b0,KP};
						8'd4		:	avs_s0_readdata	=	{16'b0,KI};
						8'd5		:	avs_s0_readdata	=	{16'b0,KD};
						default	:	avs_s0_readdata	=	32'b0;
					endcase
				end
			else
				begin
					avs_s0_readdata	=	32'b0;
				end
		end
	// Write
	always@(posedge clock_clk)
	begin
		if(reset_reset)
			begin
				RPM <= 16'b0;
			end
		else if(avs_s0_write)
			begin
				case (avs_s0_address)
					8'b0    : PWM	<= avs_s0_writedata[15:0];
					8'd1    : KP	<= avs_s0_writedata[15:0];
					8'd2    : KI	<= avs_s0_writedata[15:0];
					8'd3    : KD	<= avs_s0_writedata[15:0];
					default : PWM	<=	PWM;
				endcase
			end
	end
endmodule
