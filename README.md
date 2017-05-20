This project was developed for Cornell's ECE5725 Embedded OS class. 

The final CNC project is held in folder 6-cnc_controller.



# Compiling and runnning bare metal projects on RPI2/3

1. Install arm-none-eabi toolchain (https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) to `/usr/local/` and make sure it's in your $PATH 

2. Format SD card as fat32, named BOOT.

3. Copy over RPi's boot code (bootcode.bin and start.elf, get it from https://github.com/raspberrypi/firmware/tree/master/boot) to the root of the SD card. 

4. Run with sd card still in. First will compile a kernel.img to run, the second will copy to SD card (this is convenience target, can also easily just copy the resulting kernel.img to the SD card root).
```
> make
> make copy
```
5. Install SD card in RPi and plug in power. The kernel.img will start executing. 

# Examples:

Each of these examples build on the previous, so the last example should contain all the features, so copy code from an earlier example into a later one to use that featuer. 

1. rpi_test_project
    
This is a simple project using the ARM timer interrupt to flash an LED. Note the LED flashing only works on RPI2, for RPI3 change to a different GPIO, on the header 

2. uart_example

Demonstrates writing using the mini UART (UART 1) and a simple implementation of printf. 

