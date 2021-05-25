# Driver avalon_pwm

The avalon_pwm driver enables to read and write from the HPS to the PWM through the lightweight Avalon bus in the DE0 Nano SoC that connects directly to the FPGA of the Cyclone V.

## Files

Driver that comunicates with the PWM module implemented in the FPGA.

* avalon_pwm.c: Source code in C that defines the avalon_pwm driver.
* Kbuild: Defines the files to be built.
* Makefile: Defines set of tasks to be executed.
* user/test_avalon_pwm.c: App made for testing the driver.
* user/Makefile: Defines set of tasks to be executed for the test app.