# Flight Software for Haloship's Custom Avionics

This is PlatformIO project. To configure the project for a new board you must update the `platformio.ini` with the relevant configuration.

The Haloship flight software uses the [Arduino core provided by stm32duino](https://github.com/stm32duino/Arduino_Core_STM32). All code was tested with the [Adafruit Feather STM32F405](https://www.adafruit.com/product/4382) development board and is built for a custom STM32F405 board.


To interface with PlatformIO's command line tool, this project uses the `interface.py` file. Here is the help message from the interface file:

```
usage: interface.py [-h] [-m [MONITOR]] [-b] [-bn] [-un] [-bf] [-uf]

Interface script to execute various commands for the PIO project.

optional arguments:
  -h, --help            show this help message and exit
  -m [MONITOR], --monitor [MONITOR]
                        Serial monitor with a default baud rate of 115200
  -b, --build           Build project for all environments specified in platformio.ini
  -bn, --build_nucleo   Build firmware for NUCLEO-F411RE
  -un, --upload_nucleo  Build and upload firmware to NUCLEO-F411RE
  -bf, --build_feather  Build firmware for Feather F405
  -uf, --upload_feather
                        Build and upload firmware to Feather F405

```