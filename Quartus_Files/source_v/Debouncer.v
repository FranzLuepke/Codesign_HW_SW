// Debouncer.v
//https://eewiki.net/pages/viewpage.action?pageId=13599139

//////////////////////// Button Debounceer ///////////////////////////////////////
//***********************************************************************
// FileName: Debouncer.v
// FPGA: MachXO2 7000HE
// IDE: Diamond 2.0.1 
//
// HDL IS PROVIDED "AS IS." DIGI-KEY EXPRESSLY DISCLAIMS ANY
// WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL DIGI-KEY
// BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
// DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
// PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
// BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
// ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
// DIGI-KEY ALSO DISCLAIMS ANY LIABILITY FOR PATENT OR COPYRIGHT
// INFRINGEMENT.
//
// Version History
// Version 1.0 04/11/2013 Tony Storey
// Initial Public Release
// Small Footprint Button Debouncer

//`timescale 1 ns / 100 ps
module  Debouncer 
(
	input		DeBounce_CLOCK_50, DeBounce_Reset_InHigh, DeBounce_Button_In,	// inputs
	output reg	DeBounce_Button_Out												// output
);
	//// ---------------- internal constants --------------
	parameter N = 8 ;		// (2^ (21-1) )/ 38 MHz =  ms debounce time
	////---------------- internal variables ---------------
	reg	[N-1 : 0]	q_reg;		// timing regs
	reg	[N-1 : 0]	q_next;
	reg				DFF1, DFF2;	// input flip-flops
	wire			q_add;		// control flags
	wire			q_reset;
	//// ------------------------------------------------------
	////contenious assignment for counter control
	assign	q_reset	= (DFF1  ^ DFF2);	// xor input flip flops to look for level chage to reset counter
	assign	q_add	= ~(q_reg[N-1]);	// add to counter when q_reg msb is equal to 0
	//// combo counter to manage q_next	
	always @ ( q_reset, q_add, q_reg)
		begin
			case( {q_reset , q_add})
				2'b00	: q_next <= q_reg;
				2'b01	: q_next <= q_reg + 1'b1;
				default	: q_next <= { N {1'b0} };
			endcase 	
		end
	//// Flip flop inputs and q_reg update
	always @ ( posedge DeBounce_CLOCK_50 )
		begin
			if(DeBounce_Reset_InHigh ==  1'b1)
				begin
					DFF1	<= 1'b0;
					DFF2	<= 1'b0;
					q_reg	<= { N {1'b0} };
				end
			else
				begin
					DFF1	<= DeBounce_Button_In;
					DFF2	<= DFF1;
					q_reg	<= q_next;
				end
		end
	//// counter control
	always @ ( posedge DeBounce_CLOCK_50 )
		begin
			if(q_reg[N-1] == 1'b1)
					DeBounce_Button_Out <= DFF2;
			else
					DeBounce_Button_Out <= DeBounce_Button_Out;
		end
endmodule
