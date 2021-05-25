// avalon_encoder.v
`timescale 1 ps / 1 ps
module avalon_encoder
(
	input							avs_s0_address,     //	avs_s0.address
	input							avs_s0_read,        //	.read
	output signed	[31:0]	avs_s0_readdata,    //	.readdata
	input							avs_s0_write,       //	.write
	input  signed	[31:0]	avs_s0_writedata,   //	.writedata
	output						avs_s0_waitrequest, //	.waitrequest
	input							clock_clk,          //	clock.clk
	input							reset_reset,        //	reset.reset
	input				[31:0]	count               //	count.new_signal
);
	// ASSIGNS
	assign						avs_s0_readdata	=	returnvalue;
	assign						waitrequest			=	(waitFlag && avs_s0_read);
	// REGS
	reg				[31:0]	returnvalue;
	reg							waitFlag;
	// COMBINATIONAL LOGIC
	always @(posedge clock_clk, posedge reset_reset)
		begin: AVALON_READ_INTERFACE
			if(reset_reset == 1)
				begin
					waitFlag <= 1;
				end
			else
				begin
					waitFlag <= 1;
					if(avs_s0_read)
						begin
							case(avs_s0_address)
								1'd0		:	returnvalue <= 32'h01010101;
								default	:	returnvalue	<=	32'h0101010F;
							endcase
							if(waitFlag == 1)
								begin
									waitFlag <= 0;
								end
						end
				end
		end
endmodule
