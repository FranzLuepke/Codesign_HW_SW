# Driver avalon_control

The avalon_control driver enables to read and write from the HPS to the PID controller through the lightweight Avalon bus in the DE0 Nano SoC that connects directly to the FPGA of the Cyclone V.

## Files

Driver that comunicates with the PID controller module implemented in the FPGA.

* avalon_control.c: Source code in C that defines the avalon_control driver.
* Kbuild: Defines the files to be built.
* Makefile: Defines set of tasks to be executed.
* user/test_avalon_control.c: App made for testing the driver.
* user/Makefile: Defines set of tasks to be executed for the test app.