AM32 HW Analyzer
================

This is a tool to quickly determine the pin mappings of a BLHELI_32 brushless ESC, in order to figure out which AM32 firmware is compatible. (or find out the right compile options for a new AM32 firmware)

AM32 firmware: https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware

NOTE: This project is meant for the adventurous hacker who purchased a ESC that has not yet been confirmed to be compatible with AM32 firmware. So the instructions are a bit sparse here, as it is expected that this person has a higher than usual level of skill and experience.

Quick Instructions
------------------

Tools:
 * USB ESC Link (USB to serial converter with a bidirectional pin)
 * ST-Link (preferably v2)

First, figure out what microcontroller is on the ESC. Most of them will be the STM32F051, but there are also STM32F031 and STM32G071. Figure this out by (two ways):
 * reading the text on the microchip
 * use ST-Link Utility to read the microcontroller's internal identifier

Second, find the SWDIO and SWCLK pins on the ESC. Have a look at the photo such as [this](https://github.com/Voodoobrew101/Media/raw/main/Wraith32-35A.png) or [this](https://user-images.githubusercontent.com/125764759/219981368-0c8087ad-3744-447d-926d-7fb3dd2b3269.png) or [this](https://user-images.githubusercontent.com/43121317/227747566-2c793599-9fc1-410f-be69-1d3437afca35.jpg) as examples of what to look for. Solder wires to those signals, connect them to the ST-Link, don't forget the ground signal.

Within this project, there are prebuilt `am32-hw-analyzer.xxx.hex` files, and the file name indicates the microcontroller. You can choose the right one and flash it into the microcontroller. The instructions for this looks similar to [the instructions here](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/wiki/Crawler-AM32--single-ESC-Flashing-Tutorial), but with the `am32-hw-analyzer.xxx.hex` file.

(or, use Arduino IDE to build the `am32-hw-analyzer.ino` file, which will generate a `am32-hw-analyzer.ino.hex` somewhere on your computer. Arduino can flash this through ST-Link for you, or you can use the ST-Link utility to do the flashing.)

Connect the USB ESC link to the ESC, make sure everything has power. Open a connection to the serial port with any serial terminal software (such as Arduino, Putty, TeraTerm, RealTerm, etc) that can send things to the serial port. Use 19200 baud rate.

(at this point, make sure the motor is disconnected from the ESC)

On the screen you should periodically see the text:

> RC input is XXX - send \n (or ENTER key) to continue

Now send the `\n` or press the `ENTER` key. The analysis will begin. The results will be printed later. For example:

>     phase pin matching results:
>     1: HIN->PA10 , LIN->PB1 , FB->PA5
>     2: HIN->PA9 , LIN->PB0 , FB->PA4
>     3: HIN->PA8 , LIN->PA7 , FB->PA0

These results can be checked against [this file](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware/blob/master/Inc/targets.h) to figure out either which AM32 firmware to download, or how to configure AM32 firmware's build to compile a new firmware.

There are other things this tool can do:
 * monitor analog input voltages, to assist in finding the voltage sensing pin and current sensing pin
 * to find the current sensing pin, there is a mode that drives the motor winding to draw current, connect the motor if this mode is used
 * find pins for LEDs, by cycling through all pins and causing a blink, the user can pause the automatic cycling and manually cycle through the pins with commands

The serial terminal will display the instructions.

More Detailed Instructions
--------------------------

TODO


Arduino Setup
-------------

If you need to customize this tool's source code, then use Arduino and install [Arduino_Core_STM32](https://github.com/stm32duino/Arduino_Core_STM32). Before the build, make sure the right board and right chip is selected, and disable the serial port in the compile options.
