# Source v files

Hardware Verilog files.

## ADC_Interface

Converts a read analog signal to a 12-bit digital signal.

## Control_SDI

Serial data input of the ADC.

## Cycles_Counter

Counts the number of cycles given by parameter and activates and output flag when ending the count.

## Debouncer

Avoids noise in the signal from bouncing of the inputs.

## Encoder

Counts the edges from a quadrature (Channel A & B) encoder.

## Motors_Enable

Ouputs 1-bit enable signals based on the requested input.

## PID_Control

Receives a setpoint and measured RPM value and caculates the best PWM value with its 2-bit direction.

## Prescaler

Receives a prescaler value and outputs a whole division of the input clock based on the input value.

## PWM_Generator

Generates a pulse of the width required by the input at a fixed frequency.

## RPM

Reads the encoder counts, numerically calculates its derivative and outputs the calculated velocity in RPM.