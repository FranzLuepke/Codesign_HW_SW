# Driver avalon_encoder
The avalon_encoder driver enables to read from HPS to the encoder module through the lightweight Avalon bus in the DE0 Nano SoC that connects directly to the FPGA of the Cyclone V.

## Files
Driver that comunicates with the encoder module implemented in the FPGA.

* avalon_encoder.c: Source code in C that defines the avalon_encoder driver.
* Kbuild: Defines the files to be built.
* Makefile: Defines set of tasks to be executed.
* user/test_avalon_encodr.c: App made for testing the driver.
* user/Makefile: Defines set of tasks to be executed for the test app.