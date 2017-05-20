
ARMGNU ?= arm-none-eabi
CFLAGS = -Wall -Werror -O2 -nostartfiles -march=armv7-a -mtune=cortex-a7
LFLAGS = -L/usr/local/gcc-arm-none-eabi-5_4-2016q3/lib/gcc/arm-none-eabi/5.4.1 -lgcc

TARGET = uart
OBJDIR = obj
BCMDIR = bcm

all : setup gcc $(BCMDIR)/memmap $(BCMDIR)/rpi-base.h
gcc : $(OBJDIR)/kernel.img


setup: 
	mkdir -p $(OBJDIR)

clean :
	rm -rf $(OBJDIR)

copy :
	cp $(OBJDIR)/kernel.img /Volumes/BOOT/kernel.img

## create the binary for execution
$(OBJDIR)/kernel.img : $(OBJDIR)/$(TARGET).elf
	$(ARMGNU)-objcopy $< -O binary $@

## link objects into an elf and dump it
$(OBJDIR)/$(TARGET).elf : $(OBJDIR)/vectors.o $(OBJDIR)/$(TARGET).o $(OBJDIR)/rpi-systimer.o \
						$(OBJDIR)/rpi-gpio.o $(OBJDIR)/rpi-armtimer.o $(OBJDIR)/rpi-interrupts.o \
						$(OBJDIR)/rpi-aux.o $(OBJDIR)/printf.o ## inclue the other targets here. 
	$(ARMGNU)-ld $^ $(LFLAGS) -T $(BCMDIR)/memmap -o $@
	$(ARMGNU)-objdump -D $@ > $@.list

## compile all the source into objects
$(OBJDIR)/vectors.o : $(BCMDIR)/vectors.s
	$(ARMGNU)-as $< -o $@

$(OBJDIR)/$(TARGET).o : $(TARGET).c ## include related h files here 
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-systimer.o : $(BCMDIR)/rpi-systimer.c $(BCMDIR)/rpi-systimer.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-gpio.o : $(BCMDIR)/rpi-gpio.c $(BCMDIR)/rpi-gpio.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-armtimer.o : $(BCMDIR)/rpi-armtimer.c $(BCMDIR)/rpi-armtimer.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-interrupts.o : $(BCMDIR)/rpi-interrupts.c $(BCMDIR)/rpi-interrupts.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-aux.o : $(BCMDIR)/rpi-aux.c $(BCMDIR)/rpi-aux.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/printf.o : $(BCMDIR)/printf.c $(BCMDIR)/printf.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

## create other (C) object with this. Copy as example above for assembly 

#$(OBJDIR)/<target name> : <target source, c file first>
#	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@