# Flight Software for Haloship's Custom Avionics

## Introduction

This flight software was developed as a PlatformIO project. To port the project for a new board you must update the `platformio.ini` with the relevant configuration.

The Haloship flight software is built atop the [Arduino core provided by stm32duino](https://github.com/stm32duino/Arduino_Core_STM32). All code was tested with the [Adafruit Feather STM32F405](https://www.adafruit.com/product/4382) development board and is built for a custom STM32F405 board.

## Installation 

1. Make sure PlatformIO is installed as part of VSCode

1. Follow this [link](https://docs.platformio.org/en/latest/core/installation.html#piocore-install-shell-commands) to make sure pio is part of your PATH
```
export PATH=$PATH:$HOME/.platformio/penv/bin
```
1. Clone this [repository](https://github.com/haloship/haloship_variant)

1. Make sure the `ststm32` platform is installed in PlatformIO. [See here](https://docs.platformio.org/en/stable/platforms/ststm32.html)

1. In `~/.platformio/platforms/ststm32/boards` copy over `haloship.json` found in the `haloship_variant` repository.

1. In `~/.platformio/packages/framework-arduinoststm32/boards.txt` add: 
```
# Haloship Feather STM32F405 board
GenF4.menu.pnum.HALOSHIP_F405=Halsohip Board F405
GenF4.menu.pnum.HALOSHIP_F405.upload.maximum_size=1048576
GenF4.menu.pnum.HALOSHIP_F405.upload.maximum_data_size=131072
GenF4.menu.pnum.HALOSHIP_F405.build.board=HALOSHIP_F405
GenF4.menu.pnum.FEATHER_F405.build.product_line=STM32F405xx
GenF4.menu.pnum.HALOSHIP_F405.build.variant=HALOSHIP_F405
```

1. In `~/.platformio/packages/framework-arduinoststm32/variants/` create a directory named `HALOSHIP_F405`. Into this folder copy: `ldscript.ld`, `PeripheralPins.c`, `PinNamesVar.h`, `variant.cpp`, `variant.h`

## Basic Usage

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

You can extend this interface file for additional environments defined in `platformio.ini`

## Other Usage

Pinouts for each board are defined in `src/config.hpp`. If you are using a development board already defined in the configuration file, then match the pinouts shown in the file for each module. Otherwise, you may redefine the pinouts here to match your preferred setup.