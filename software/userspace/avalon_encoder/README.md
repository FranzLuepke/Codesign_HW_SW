# App avalon_encoder
The avalon_encoder app enables to read quadrature encoders connected to FPGA GPIO pins through the *mem* device.

## Files
Driver that comunicates with the encoder module connected to the FPGA in the DE0 Nano SoC development board.

* devmem_avalon_encoder.c: Source code in C that writes to the */dev/mem* where the encoder IP component is connected.
* Makefile: Defines set of tasks to be executed.