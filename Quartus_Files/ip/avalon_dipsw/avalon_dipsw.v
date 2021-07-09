// avalon_dipsw.v
`timescale 1 ps / 1 ps
module avalon_dipsw (
		input  wire				avs_s0_address,		//	avs_s0.address
		input  wire				avs_s0_read,			//	.read
		output wire [31:0]	avs_s0_readdata,		//	.readdata
		output wire				avs_s0_waitrequest,	//	.waitrequest
		input  wire				clock_clk,				//	clock.clk
		input  wire				reset_reset,			//	reset.reset
		input  wire [3:0]		dipsw,					//	dipsw.new_signal
		output reg				irq						//	interrupt_sender.irq
	);
	// ASSIGNS
	assign						avs_s0_readdata		=	returnvalue;
	assign						avs_s0_waitrequest	=	(waitFlag && avs_s0_read);
	// REGS
	reg							waitFlag;
	reg				[3:0]		last_state;
	reg				[31:0]	returnvalue;
	// COMBINATIONAL LOGIC
	always @(posedge clock_clk, posedge reset_reset)
		begin: AVALON_READ_INTERFACE
			if(reset_reset == 1)
				begin
					waitFlag	<= 1;
					irq		<= 0;
				end
			else
				begin
					waitFlag <= 1;
					if(avs_s0_read)
						begin
							case(avs_s0_address)
								1'd0		:	returnvalue <= {28'd0,dipsw};
								default	:	returnvalue <= 32'd0;
							endcase
							if(waitFlag == 1)
								begin
									waitFlag		<= 0;
									irq			<= 1'b0;
								end
							if(last_state != dipsw)
								begin
									irq			<= 1;
									last_state	<= dipsw;
								end
						end
				end
		end
endmodule
