// avalon_rpm.v
`timescale 1 ps / 1 ps
module avalon_rpm
(
	input  wire	[7:0]		avs_s0_address,     //	avs_s0.address
	input  wire				avs_s0_read,        //	.read
	output reg	[31:0]	avs_s0_readdata,    //	.readdata
	input  wire				avs_s0_write,       //	.write
	input  wire	[31:0]	avs_s0_writedata,   //	.writedata
	output wire				avs_s0_waitrequest, //	.waitrequest
	input  wire				clock_clk,          //	clock.clk
	input  wire				reset_reset,        //	reset.reset
	input  wire	[15:0]	rpm,                //	rpm.new_signal
	output reg	[15:0]	cpr,                //	cpr.new_signal
	output reg	[15:0]	gearhead            //	gearhead.new_signal
);
	assign						avs_s0_waitrequest = 1'b0;
	// COMBINATIONAL LOGIC
	// Read
	always@(avs_s0_read)
		begin
			if(avs_s0_read)
				begin
					case(avs_s0_address)
						8'd0		:	avs_s0_readdata	=	{16'b0,rpm};
						8'd1		:	avs_s0_readdata	=	{16'b0,cpr};
						8'd2		:	avs_s0_readdata	=	{16'b0,gearhead};
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
			if(avs_s0_write)
				begin
					case (avs_s0_address)
						8'b0    : cpr			<= avs_s0_writedata[15:0];
						8'd1    : gearhead	<= avs_s0_writedata[15:0];
						default : cpr			<=	cpr;
					endcase
				end
		end
endmodule
