// avalon_enable.v
`timescale 1 ps / 1 ps
module avalon_enable
(
	// Avalon Memory Mapped Slave
	input  wire [3:0]  avs_s0_address,     //	avs_s0.address
	input  wire        avs_s0_read,        //	.read
	output reg	[31:0] avs_s0_readdata,    //	.readdata
	input  wire        avs_s0_write,       //	.write
	input  wire [31:0] avs_s0_writedata,   //	.writedata
	output wire        avs_s0_waitrequest, //	.waitrequest
	// Clock and reset
	input  wire        clock_clk,          //	clock.clk
	input  wire        reset_reset,        //	reset.reset
	// Custom signals
	input  wire [7:0]  enable,             //	enable.new_signal
	output reg	[7:0]  motor_select,       //	motor_select.new_signal
	output reg	[1:0]  state               //	state.new_signal
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
					4'd0		:	avs_s0_readdata	=	{24'b0, enable};
					4'd1		:	avs_s0_readdata	=	{24'b0, motor_select};
					4'd2		:	avs_s0_readdata	=	{30'b0, state};
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
				motor_select <= 16'b0;
			end
		else if(avs_s0_write)
			begin
				case (avs_s0_address)
					4'b0    : motor_select	<= avs_s0_writedata[7:0];
					4'd1    : state			<= avs_s0_writedata[2:0];
					default : motor_select	<=	motor_select;
				endcase
			end
	end
endmodule
