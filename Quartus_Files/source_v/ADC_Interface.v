module ADC_Interface(clock, ADC_CONVST, ADC_SCK, ADC_SDI, ADC_SDO, MUX_CONTROL, CH0, CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12);
	// INPUTS AND OUTPUTS
	input clock, ADC_SDO;
	output ADC_CONVST, ADC_SCK, ADC_SDI;
	output [2:0] MUX_CONTROL;
	output [11:0] CH0, CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12 = 12'd0;
	// Control SDI external module
	Control_SDI ctSDI1(flag, ADC_SCK, ADC_SDI, CH_ACTUAL, MUX_CONTROL);
	// REGS & WIRES
	reg [11:0] datosRecibidos = 0;
	reg [11:0] CH0, CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12;
	reg ADC_CONVST;
	reg rst = 1;
	wire flag;
	wire [3:0] CH_ACTUAL;
	// EXTERNAL MODULES
	Prescaler prescaler(clock, rst, 250, ADC_SCK);
	Cycles_Counter pos_edge_counter(clock, rst, 6100, flag);
	// PARAMETERS
	parameter STATE_0=2'd0;
	parameter STATE_1=2'd1;
	parameter STATE_2=2'd2;
	// REGS
	reg [1:0] actual_state=STATE_0;
	reg [1:0] future_state=STATE_0;
	//Logica secuencial
	always@(posedge clock)
		begin
			if(flag)
				begin
					actual_state<=STATE_0;
					case(CH_ACTUAL)
						4'd0: CH0 <= datosRecibidos;
						4'd1: CH1 <= datosRecibidos;
						4'd2: CH2 <= datosRecibidos;
						4'd3: CH3 <= datosRecibidos;
						4'd4: CH4 <= datosRecibidos;
						4'd5: CH5 <= datosRecibidos;
						4'd6: CH6 <= datosRecibidos;
						4'd7: CH7 <= datosRecibidos;
						4'd8: CH8 <= datosRecibidos;
						4'd9: CH9 <= datosRecibidos;
						4'd10: CH10 <= datosRecibidos;
						4'd11: CH11 <= datosRecibidos;
						4'd12: CH12 <= datosRecibidos;
						default:
						begin
							CH0 <= CH0;
							CH1 <= CH1;
							CH2 <= CH2;
							CH3 <= CH3;
							CH4 <= CH4;
							CH5 <= CH5;
							CH6 <= CH6;
							CH7 <= CH7;
							CH8 <= CH8;
							CH9 <= CH9;
							CH10 <= CH10;
							CH11 <= CH11;
							CH12 <= CH12;
						end
					endcase
				end
			else
				begin
					actual_state<=future_state;
					CH0 <= CH0;
					CH1 <= CH1;
					CH2 <= CH2;
					CH3 <= CH3;
					CH4 <= CH4;
					CH5 <= CH5;
					CH6 <= CH6;
					CH7 <= CH7;
					CH8 <= CH8;
					CH9 <= CH9;
					CH10 <= CH10;
					CH11 <= CH11;
					CH12 <= CH12;
				end
		end
	// SEQUENTIAL NEW CLOCK
	always @ (posedge ADC_SCK)
		begin
			datosRecibidos<={datosRecibidos[10:0], ADC_SDO};
		end
	// States combitional logic
	always@(*)
		begin
			case(actual_state)
				STATE_0:begin
								future_state=STATE_1;
							end
				STATE_1:begin
								future_state=STATE_2;
							end
				STATE_2:begin
								future_state=STATE_2;
							end
				default:future_state=STATE_0;
			endcase
		end
	// Output combitional logic
	always@(*)
		begin
			case(actual_state)
				STATE_0: 
					begin
						rst = 1;
						ADC_CONVST = 0;
					end
				STATE_1: 
					begin
						rst = 1;
						ADC_CONVST = 1;
					end
				STATE_2: 
					begin
						rst = 0;
						ADC_CONVST = 0;
					end
				default: 
					begin
						rst = 1;
						ADC_CONVST = 0;
					end
			endcase
		end
endmodule
