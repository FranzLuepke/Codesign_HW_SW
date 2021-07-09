# Driver avalon_enable

The avalon_enable driver enables to read and write, from the HPS, the enable modules through the lightweight Avalon bus in the DE0 Nano SoC that connects directly to the FPGA of the Cyclone V.

## Files

Driver that comunicates with the enable module implemented in the FPGA.

* avalon_enable.c: Source code in C that defines the avalon_enable driver.
* Kbuild: Defines the files to be built.
* Makefile: Defines set of tasks to be executed.
* user/test_avalon_enable.c: App made for testing the driver.
* user/Makefile: Defines set of tasks to be executed for the test app.