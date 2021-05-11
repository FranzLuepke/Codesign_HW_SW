// avalon_leds.v
`timescale 1 ps / 1 ps
module avalon_leds
(
	input		wire				clk,							//	clock.clk
	input		wire				reset,						//	reset.reset
	input		wire				avalon_slave_address,	//	avalon_slave.address
	input		wire				avalon_slave_read,		//	.read
	input		wire				avalon_slave_write,		//	.write
	output	wire	[31:0]	avalon_slave_readdata,	//	.readdata
	input		wire	[31:0]	avalon_slave_writedata,	//	.writedata
	output	wire	[7:0]		leds							//	count.new_signal
);
	// Reade operations performed on the Avalon-MM Slave interface
	always@(*)
		begin
			if(avalon_slave_read)
				begin
					case(avalon_slave_address)
						assign	:	avalon_slave_readdata	=	{26'b0,leds};
						default	:	avalon_slave_readdata	=	32'b0;
					endcase
				end
			else
				begin
					avalon_slave_readdata	=	32'b0;
				end
		end
	// Write operations performed on the Avalon-MM Slave interface
	always@(posedge clk)
		begin
			if(reset)
				begin
					leds <= 8b'0;
				end
			else
				if(avalon_slave_write)
					begin
						case(avalon_slave_address)
							1'b0    : leds <= avalon_slave_writedata[7:0];
							default : leds <= leds;
						endcase
					end
		end
endmodule
