# Kernelspace

Drivers to work with the FPGA synthetized hardware.

## Custom LEDs

Driver that comunicates with the LEDs connected to the FPGA in the DE0 Nano SoC development board. For more documentation read the README.md file inside the custom_leds folder.

## Avalon LEDs

Driver that comunicates with the LEDs connected to the FPGA in the DE0 Nano SoC development board. For more documentation read the README.md file inside the avalon_leds folder.

## Avalon PWM

Driver that comunicates with the PWM modules in the FPGA. For more documentation read the README.md file inside the avalon_pwm folder.

## Avalon Encoder

Driver that comunicates with the the quadrature encoder modules in the FPGA. For more documentation read the README.md file inside the avalon_encoder folder.

## Avalon Control

Driver that comunicates with the PID controller modules in the FPGA. For more documentation read the README.md file inside the avalon_control folder.


# How to use the drivers

To use a kernel module you must have the kernel headers necessary for your kernel version.

> ### In the host
1. Set KERNEL_PATH enviroment variable to your kernel headers path:
> > `export KERNEL_PATH=<your-kernel-path>`
2. To compile the program from the source code use the command:
> > `make`
3. Copy the *<kernel-module>* binary file to 
> > `scp <kernel-module>.ko <user>@<ip>:<driver-path>`
> > > ***Note**: The recommended user is root.*

> ### In the target
1. Go to the driver path:
> > `cd <driver-path>`
2. Insert the kernel module:
> > ` insmod <kernel-module>.ko`
4. Check if the kernel module is listed
> > `lsmod`
3. Write to the device: (`\x55` can be changed for other desired value.)
> > `echo -n -e '\x55' > /dev/<device-name>`
4. Remove the kernel module:
> > ` rmmod <kernel-module>`

> ### Additional commands
* To clean the compiled files use:
> > `make clean`
* To ask for help use:
> > `make help`