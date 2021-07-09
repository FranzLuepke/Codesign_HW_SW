# Driver avalon_dipsw
The avalon_dipsw driver enables to read from HPS the dip switches state through the lightweight Avalon bus in the DE0 Nano SoC that connects directly to the FPGA of the Cyclone V.

## Files
Driver that comunicates with the dip switches connected to the FPGA.

* avalon_dipsw.c: Source code in C that defines the avalon_dipsw driver.
* Kbuild: Defines the files to be built.
* Makefile: Defines set of tasks to be executed.
* user/test_avalon_dipsw.c: App made for testing the driver.
* user/Makefile: Defines set of tasks to be executed for the test app.