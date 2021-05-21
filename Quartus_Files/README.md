# Quartus Files

This folder contains the hardware project files for the development board DE0 Nano SoC.

## Main files and folders description

### Files
* **soc_system.qpf:** Main Quartus project file.
* **soc_system.qsys:** Platform Designer System File.
* **ghrd.v:** Main source file. Here occur the interconnections between the soc_system (HPS) and the hardware modules (FPGA).

### Folders
* **ip:** IP component files. These are the custom IP components created to communicate the FPGA and the HPS.
* **source_v:** Source v files. These are the hardware files needed to compile the FPGA.
* **soc_system:** Qsys files.

## Custom IP components
These components are reusable design blocks that all for fast and easy integration of functions and can be graphically connected using Platform Designer to integrate functions from the HPS and the FPGA.

The custom components created in this project are the following:

* **avalon_leds:** Allows to read and write to the LEDs integrated in the DE0 Nano SoC Development Board.
* **avalon_pwm:** Allows to write a desired PWM as a setpoint and reads the calculated PWM.
* **avalon_encoder:** Allows to read the quadrature encoder count.