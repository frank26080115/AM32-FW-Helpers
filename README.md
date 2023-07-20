# AM32-FW-Helpers
Helpful tools for AM32 firmware

[AM32 firmware](https://github.com/AlkaMotors/AM32-MultiRotor-ESC-firmware) is an open source firmware project for brushless motor ESCs. These helpful tools I've created are meant to assist in both the utilization and the development of AM32 firmware.

 * am32-image-generator
   * used to stitch together the bootloader, FW, and EEPROM files, into one big *.hex file for faster repeated batch flashing
   * Python script
 * am32-eeprom-extractor
   * used to extract the EEPROM binary file out of a FW dump
   * Python script
 * am32-eeprom-tools
   * used to edit the EEPROM of a ESC running AM32
   * able to load and save from file
   * able to upload and download EEPROM data from the ESC through serial port
   * Webpage app (HTML and JavaScript), can be used offline as well as online
 * am32-hw-analyzer
   * STM32 firmware that can help figure out the pin mappings of an ESC that has not yet been confirmed to be supported by AM32
   * Arduino sketch
 * am32-fw-updater
   * write firmware hex file into ESC through the serial port bootloader
   * Python script
 * am32-fw-dumper
   * dump firmware bin file from ESC through the serial port bootloader
   * Python script, found inside the am32-fw-updater directory
