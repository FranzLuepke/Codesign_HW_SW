# App avalon_leds

The avalon_leds app enables to write to the LEDs in the FPGA through the *mem* device.

## Files

Driver that comunicates with the LEDs connected to the FPGA in the DE0 Nano SoC development board.

* devmem_avalon_leds.c: Source code in C that writes to the */dev/mem* where the LED IP component is connected.
* Makefile: Defines set of tasks to be executed.

## How to use

To use this kernel module you must have the kernel headers necessary for your kernel version.

> ### In the host
1. Set CROSS_COMPILE enviroment variable to your toolchain's binary path:
> > `export CROSS_COMPILE=<your-toolchain-path>/bin/arm-linux-gnueabihf-`
2. To compile the program from the source code use the command:
> > `make`
3. Copy the *devmem_custom_leds* binary file to 
> > `scp devmem_custom_leds <user>@<ip>:<binary-path>`
> > > ***Note**: The recommended user is root.*

> ### In the target
1. Go to the binary path:
> > `cd <binary-path>`
2. Run the execuable with a number of blinking times as argument:
> > `.\devmem_custom_leds <blinking-times>`
3. Watch the LEDs blinking in the DE0 Nano SoC.

> #### Additional commands
* To clean the compiled files use:
> > `make clean`
* To ask for help use:
> > `make help`