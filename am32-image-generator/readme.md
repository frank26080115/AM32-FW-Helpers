This is a tool to combine binary images for ESCs that run [AM32](https://am32.ca) firmware. The ESCs need 3 flash sections: bootloader, firmware, configuration (aka EEPROM). This tool combines all three into one file, which speeds up the flashing process for manufacturers or anybody who needs an archive firmware.

This tool is meant for people who have expertise in microcontroller firmware.

# Requirements

Install Python 3.7 or later.

Required library: intelhex https://pypi.org/project/intelhex/, use `pip install intelhex` to install

# Execution

Usage guide:

```
python am32-image-generator.py [-h] [-r] [-n] [-a baseaddr] [-e eeprom] [-x eepromaddr] [-m mcu] [--singlechunk]
                               [-o outpath] [-v]
                               bootloader firmware
```

For example:

```
python am32-image-generator.py -o outputfilename.hex bootloaderfilepath.hex firmwarefilepath.hex
```

Another example:

```
python am32-image-generator.py -m f051 -o outputfilename.hex bootloaderfilepath.hex firmwarefilepath.hex
```

The bootloader file and firmware file are required. Both of these files can be either `*.bin` or `*.hex` files.

To specify a EEPROM (aka configuration) file, use the `-e <file path>` option.

To specify the file name (or path) for the output, specify with `-o <file path>`. It must end with `.hex`. If this is not specified, a file name will automatically be generated.

The most important option is `-mcu`, it accepts the following values:
`f051` / `f031` / `gde230` / `g071-64k` / `l431` / `at421` / `atf415` / `g031` / `g431` / `v203` / `g071-128k` / `atf415-128k`. This parameter automatically fills the `-x` option. This option is required if you are using `g071-128k`.

The option `-a` is defaulted to `0x08000000` which is correct for most STM32 microcontrollers.

The option `-x` is defaulted to `0x7C00` which is correct for microcontrollers with 32KB of memory. You don't need to specify this if you use `-mcu`

If your flashing tool does not support `*.hex` files with multiple internal chunks, then you need to specify the `--singlechunk` option to make sure the final file contains only one gigantic continuous chunk.

The option `-n` will erase the EEPROM version code from the EEPROM. This forces the firmware to override the EEPROM reported version string.
