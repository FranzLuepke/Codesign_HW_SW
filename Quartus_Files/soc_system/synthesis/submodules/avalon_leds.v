// avalon_leds.v
`timescale 1 ps / 1 ps
module avalon_leds
(
	input  wire        avs_s0_address,     //	avs_s0.address
	input  wire        avs_s0_read,        //	.read
	output reg	[31:0] avs_s0_readdata,    //	.readdata
	input  wire        avs_s0_write,       //	.write
	input  wire [31:0] avs_s0_writedata,   //	.writedata
	output wire        avs_s0_waitrequest, //	.waitrequest
	input  wire        clock_clk,          //	clock.clk
	input  wire        reset_reset,        //	reset.reset
	output reg	[7:0]  leds                //	leds.new_signal
);
	// REGS
	reg			busy		= 1'b0;
	// ASSIGNS
	assign avs_s0_waitrequest = busy;
	// READ
	always@(*)
		begin
			if(avs_s0_read)
				begin
					case(avs_s0_address)
						1'b0		:	avs_s0_readdata	=	{26'b0,leds};
						default	:	avs_s0_readdata	=	32'b0;				
					endcase
				end
		end
	// WRITE
	always@(posedge clock_clk)
		begin
			if(reset_reset)
				begin
					leds <= 8'b0;
				end
			else if(avs_s0_write)
				begin
					case (avs_s0_address)
						1'b0    : leds <= avs_s0_writedata[7:0];
						default : leds <= leds;
					endcase
				end
		end
endmodule
