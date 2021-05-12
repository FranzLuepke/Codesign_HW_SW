# Codesign_HW_SW

This repository includes a hardware software codesign project for the development board DE0 Nano SoC.

## Getting Started

1. Clone the repository
2. Open the soc_system.qpf with Quartus and configure the target board. (DE0 Nano-SoC is pre-configured)
3. In Quartus open the soc_system.qsys with Platform Designer and generate the HDL.
4. Open the tcl scripts and run the hps_sdram_p0_parameters.tcl and the hps_sdram_p0_pin_assignments.tcl files.
5. Synthetize the project.
6. Generate the .rbf (raw binary file) and the .dtb (device tree blob) files.
7. Copy the new files to the SD card.
8. Turn on the development board.
9. Compile the user or kernel files.
10. Install the software files in the board.
11. Run the binaries in the board.

### Prerequisites

* DE0 Nano SoC (other Cyclone V SoC may work)
* Quartus Prime Lite 19.1.0
* Intel SoC FPGA Embedded Development Suite
* Arm* Development Studio for Intel® SoC FPGA (not necessary but recommended)

## Built With

* [Quartus II Lite](https://www.intel.com/content/www/us/en/programmable/downloads/download-center.html) - Intel FPGAs and Programmable Devices
* [Altera ModelSim](https://www.intel.la/content/www/xl/es/software/programmable/quartus-prime/model-sim.html) - Simulation Software by Intel

## Authors

* Franz Luepke. - *Initial work, hardware modifications and software implementation* - [FranzLuepke](https://github.com/FranzLuepke)
* Oscar Díaz Caballero. - *Hardware modules* - [alegis277](https://github.com/alegis277)

## License

This project is licensed under the terms of the MIT license.