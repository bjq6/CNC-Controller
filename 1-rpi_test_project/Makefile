
ARMGNU ?= arm-none-eabi
CFLAGS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding -march=armv7-a -mtune=cortex-a7

TARGET = rpi_test
OBJDIR = obj

all : setup gcc memmap
gcc : $(OBJDIR)/kernel.img


setup: 
	mkdir -p $(OBJDIR)

clean :
	rm -rf $(OBJDIR)/

copy :
	cp $(OBJDIR)/kernel.img /Volumes/BOOT/kernel.img

## create the binary for execution
$(OBJDIR)/kernel.img : $(OBJDIR)/$(TARGET).elf
	$(ARMGNU)-objcopy $< -O binary $@

## link objects into an elf and dump it
$(OBJDIR)/$(TARGET).elf : $(OBJDIR)/vectors.o $(OBJDIR)/$(TARGET).o $(OBJDIR)/rpi-systimer.o $(OBJDIR)/rpi-gpio.o $(OBJDIR)/rpi-armtimer.o $(OBJDIR)/rpi-interrupts.o## inclue the other targets here. 
	$(ARMGNU)-ld $^ -T memmap -o $@
	$(ARMGNU)-objdump -D $@ > $@.list

## compile all the source into objects
$(OBJDIR)/vectors.o : vectors.s
	$(ARMGNU)-as $< -o $@

$(OBJDIR)/$(TARGET).o : $(TARGET).c ## include related h files here 
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-systimer.o : rpi-systimer.c rpi-systimer.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-gpio.o : rpi-gpio.c rpi-gpio.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-armtimer.o : rpi-armtimer.c rpi-armtimer.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-interrupts.o : rpi-interrupts.c rpi-interrupts.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

## create other (C) object with this. Copy as example above for assembly 

#$(OBJDIR)/<target name> : <target source, c file first>
#	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@