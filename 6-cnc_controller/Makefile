
ARMGNU ?= arm-none-eabi
AFLAGS = -mfpu=neon-vfpv4 -mfloat-abi=hard
CFLAGS = -Wall -Werror -O2 -nostartfiles -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -fdata-sections -ffunction-sections
LFLAGS = -L/usr/local/gcc-arm-none-eabi-5_4-2016q3/lib/gcc/arm-none-eabi/5.4.1/fpu -L/usr/local/gcc-arm-none-eabi-5_4-2016q3/arm-none-eabi/lib/fpu -lgcc -lc -lm

TARGET = cnc_controller
OBJDIR = obj
BCMDIR = bcm

all : setup gcc $(BCMDIR)/memmap $(BCMDIR)/rpi-base.h $(BCMDIR)/bcm-includes.h 
gcc : $(OBJDIR)/kernel.img copy

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
$(OBJDIR)/$(TARGET).elf : $(OBJDIR)/vectors.o $(OBJDIR)/$(TARGET).o $(OBJDIR)/rpi-util.o \
						$(OBJDIR)/rpi-systimer.o $(OBJDIR)/rpi-gpio.o $(OBJDIR)/rpi-armtimer.o \
						$(OBJDIR)/rpi-interrupts.o $(OBJDIR)/rpi-aux.o $(OBJDIR)/rpi-mailbox.o \
						$(OBJDIR)/rpi-mailbox-interface.o $(OBJDIR)/rpi-multicore.o $(OBJDIR)/syscalls.o \
						$(OBJDIR)/stepper_driver.o $(OBJDIR)/queue.o $(OBJDIR)/parser.o \
						$(OBJDIR)/nuts_bolts.o## inclue the other targets here. $(OBJDIR)/printf.o

	$(ARMGNU)-gcc $^ $(CFLAGS) $(LFLAGS) -T $(BCMDIR)/memmap -o $@
	$(ARMGNU)-objdump -D $@ > $@.list

## compile all the source into objects
## BCM/Main ARM stuff
$(OBJDIR)/vectors.o : $(BCMDIR)/vectors.s
	$(ARMGNU)-as  $(AFLAGS) $< -o $@

$(OBJDIR)/$(TARGET).o : $(TARGET).c ## include related h files here 
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-util.o : $(BCMDIR)/rpi-util.c $(BCMDIR)/rpi-base.h
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

$(OBJDIR)/rpi-mailbox.o : $(BCMDIR)/rpi-mailbox.c $(BCMDIR)/rpi-mailbox.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-mailbox-interface.o : $(BCMDIR)/rpi-mailbox-interface.c $(BCMDIR)/rpi-mailbox-interface.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/rpi-multicore.o : $(BCMDIR)/rpi-multicore.c $(BCMDIR)/rpi-multicore.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@
	
$(OBJDIR)/syscalls.o : $(BCMDIR)/syscalls.c
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

## Actual program code
$(OBJDIR)/stepper_driver.o : stepper_driver.c stepper_driver.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/queue.o : interpreter/queue.c interpreter/queue.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/parser.o : interpreter/parser.c interpreter/parser.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(OBJDIR)/nuts_bolts.o : interpreter/nuts_bolts.c interpreter/nuts_bolts.h
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

## create other (C) object with this. Copy as example above for assembly 

#$(OBJDIR)/<target name> : <target source, c file first>
#	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@