// avalon_control.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module avalon_control (
		input  wire [7:0]  avs_s0_address,     // avs_s0.address
		input  wire        avs_s0_read,        //       .read
		output wire [31:0] avs_s0_readdata,    //       .readdata
		input  wire        avs_s0_write,       //       .write
		input  wire [31:0] avs_s0_writedata,   //       .writedata
		output wire        avs_s0_waitrequest, //       .waitrequest
		input  wire        clock_clk,          //  clock.clk
		input  wire        reset_reset,        //  reset.reset
		output wire [15:0] RPM,                //    RPM.new_signal
		input  wire [15:0] PWM,                //    PWM.new_signal
		output wire [15:0] KP,                 //    PID.new_signal
		output wire [15:0] KI,                 //       .new_signal_1
		output wire [15:0] KD,                 //       .new_signal_2
		input  wire        DIR                 //    DIR.new_signal
	);

	// TODO: Auto-generated HDL template

	assign avs_s0_readdata = 32'b00000000000000000000000000000000;

	assign avs_s0_waitrequest = 1'b0;

	assign RPM = 16'b0000000000000000;

	assign KI = 16'b0000000000000000;

	assign KD = 16'b0000000000000000;

	assign KP = 16'b0000000000000000;

endmodule
