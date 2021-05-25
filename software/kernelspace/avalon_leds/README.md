# Driver avalon_leds

The avalon_leds driver enables to read and write from the HPS to the LEDs through the lightweight Avalon bus in the DE0 Nano SoC that connects directly to the FPGA of the Cyclone V.

## Files

Driver that comunicates with the LEDs connected to the FPGA in the DE0 Nano SoC development board.

* avalon_leds.c: Source code in C that defines the custom_leds driver.
* Kbuild: Defines the files to be built.
* Makefile: Defines set of tasks to be executed.
* user/test_avalon_leds.c: App made for testing the driver.
* user/Makefile: Defines set of tasks to be executed for the test app.

## How to use

To use this kernel module you must have the kernel headers necessary for your kernel version.

> ### In the host
1. Set KERNEL_PATH enviroment variable to your kernel headers path:
> > `export KERNEL_PATH=<your-kernel-path>`
2. To compile the program from the source code use the command:
> > `make`
3. Copy the *custom_leds* binary file to 
> > `scp custom_leds.ko <user>@<ip>:<driver-path>`
> > > ***Note**: The recommended user is root.*

> ### In the target
1. Go to the driver path:
> > `cd <driver-path>`
2. Insert the kernel module:
> > ` insmod custom_leds.ko`
> > > ***Note**: When performing this action the LEDs must turn on if everything's working correctly.*
4. Check if the kernel module is listed
> > `lsmod`
3. Write to the device: (`\x55` can be changed for other desired value.)
> > `echo -n -e '\x55' > /dev/custom_leds`
4. Remove the kernel module:
> > ` rmmod custom_leds`
> > > ***Note**: When performing this action the LEDs must turn off if everything's working correctly.*


> ### Additional commands
* To clean the compiled files use:
> > `make clean`
* To ask for help use:
> > `make help`