#ifndef RPI_GPIO_H
#define RPI_GPIO_H

#include "rpi-base.h"

/***** how to use GPIOs: ******
	
This header is not complete in terms of definitions. Only setting outputs/reading input is well defined. 
The rest must be done manually. All registers are defined though

1. set GPIO mode: gpio[GPIOx_FSEL] |= mode << GPIOx_FBIT
	where x is GPIO number, mode is the mode value (0 is input, 1 is output, others are defined in BCM manual)

2. set the output: gpio[GPIO_GPSETy] |= 1 << GPIOx
	where x is the GPIO number, y is the corresponding register (0 for GPIO 0-31, 1 for 32-53)

3. clear outputs: gpio[GPIO_GPCLRy] |= 1 << GPIOx
	x is gpio number, y is as above

4. read a gpio: gpio[GPIO_GPLEVy] & (1 << GPIOx) will be 0 when low.

Other registers hold info on pull up/down info, edge detection, value detection, etc. 

******************************/

/* The base address of the GPIO peripheral (ARM Physical Address) */
#define GPIO_BASE       (PERIPHERAL_BASE + 0x200000UL)

#define GPIO_GPFSEL0    0
#define GPIO_GPFSEL1    1
#define GPIO_GPFSEL2    2
#define GPIO_GPFSEL3    3
#define GPIO_GPFSEL4    4
#define GPIO_GPFSEL5    5

#define GPIO_GPSET0     7
#define GPIO_GPSET1     8

#define GPIO_GPCLR0     10
#define GPIO_GPCLR1     11

#define GPIO_GPLEV0     13
#define GPIO_GPLEV1     14

#define GPIO_GPEDS0     16
#define GPIO_GPEDS1     17

#define GPIO_GPREN0     19
#define GPIO_GPREN1     20

#define GPIO_GPFEN0     22
#define GPIO_GPFEN1     23

#define GPIO_GPHEN0     25
#define GPIO_GPHEN1     26

#define GPIO_GPLEN0     28
#define GPIO_GPLEN1     29

#define GPIO_GPAREN0    31
#define GPIO_GPAREN1    32 

#define GPIO_GPAFEN0    34
#define GPIO_GPAFEN1    35

#define GPIO_GPPUD      37
#define GPIO_GPPUDCLK0  38
#define GPIO_GPPUDCLK1  39

#define ACT_LED_GPFSEL      GPIO_GPFSEL4
#define ACT_LED_GPFBIT      GPIO47_FBIT
#define ACT_LED_GPSET       GPIO_GPSET1
#define ACT_LED_GPCLR       GPIO_GPCLR1
#define ACT_LED_GPIO_BIT    GPIO47
#define ACT_LED_OFF()       do { _get_gpio_reg()[ACT_LED_GPCLR] = ( 1 << ACT_LED_GPIO_BIT ); } while(0)
#define ACT_LED_ON()       	do { _get_gpio_reg()[ACT_LED_GPSET] = ( 1 << ACT_LED_GPIO_BIT ); } while(0)

#define PWR_LED_GPFSEL      GPIO_GPFSEL3
#define PWR_LED_GPFBIT      GPIO35_FBIT
#define PWR_LED_GPSET       GPIO_GPSET1
#define PWR_LED_GPCLR       GPIO_GPCLR1
#define PWR_LED_GPIO_BIT    GPIO35
#define PWR_LED_ON()        do { _get_gpio_reg()[PWR_LED_GPCLR] = ( 1 << PWR_LED_GPIO_BIT ); } while(0)
#define PWR_LED_OFF()       do { _get_gpio_reg()[PWR_LED_GPSET] = ( 1 << PWR_LED_GPIO_BIT ); } while(0)


#define FS_INPUT		0
#define FS_OUTPUT		1
#define FS_ALT5			2
#define FS_ALT4			3
#define FS_ALT0			4
#define FS_ALT1			5
#define FS_ALT2			6
#define FS_ALT3			7

#define GPIO_POFF		0
#define GPIO_PDOWN 		1
#define GPIO_PUP 		2


#define GPIO_BANK0		0
#define GPIO_BANK1		1

//Bank 0
#define GPIO0_FSEL		GPIO_GPFSEL0	
#define GPIO0_LEV		GPIO_GPLEV0
#define GPIO0_FBIT 		0
#define GPIO0 	 		0

#define GPIO1_FSEL		GPIO_GPFSEL0	
#define GPIO1_FBIT 		3
#define GPIO1 	 		1

#define GPIO2_FSEL		GPIO_GPFSEL0	
#define GPIO2_FBIT 		6
#define GPIO2 	 		2

#define GPIO3_FSEL		GPIO_GPFSEL0		
#define GPIO3_FBIT 		9
#define GPIO3 			3

#define GPIO4_FSEL		GPIO_GPFSEL0		
#define GPIO4_FBIT 		12
#define GPIO4 			4

#define GPIO5_FSEL		GPIO_GPFSEL0		
#define GPIO5_FBIT 		15
#define GPIO5 			5

#define GPIO6_FSEL		GPIO_GPFSEL0		
#define GPIO6_FBIT 		18
#define GPIO6 			6

#define GPIO7_FSEL		GPIO_GPFSEL0		
#define GPIO7_FBIT 		21
#define GPIO7 			7

#define GPIO8_FSEL		GPIO_GPFSEL0		
#define GPIO8_FBIT 		24
#define GPIO8 			8

#define GPIO9_FSEL		GPIO_GPFSEL0		
#define GPIO9_FBIT 		27
#define GPIO9 			9

// Bank 1
#define GPIO10_FSEL		GPIO_GPFSEL1	
#define GPIO10_FBIT 	0
#define GPIO10 	 		10

#define GPIO11_FSEL		GPIO_GPFSEL1	
#define GPIO11_FBIT 	3
#define GPIO11 	 		11

#define GPIO12_FSEL		GPIO_GPFSEL1	
#define GPIO12_FBIT 	6
#define GPIO12 	 		12

#define GPIO13_FSEL		GPIO_GPFSEL1		
#define GPIO13_FBIT 	9
#define GPIO13 			13

#define GPIO14_FSEL		GPIO_GPFSEL1		
#define GPIO14_FBIT 	12
#define GPIO14 			14

#define GPIO15_FSEL		GPIO_GPFSEL1		
#define GPIO15_FBIT 	15
#define GPIO15 			15

#define GPIO16_FSEL		GPIO_GPFSEL1		
#define GPIO16_FBIT 	18
#define GPIO16 			16

#define GPIO17_FSEL		GPIO_GPFSEL1		
#define GPIO17_FBIT 	21
#define GPIO17 			17

#define GPIO18_FSEL		GPIO_GPFSEL1		
#define GPIO18_FBIT 	24
#define GPIO18 			18

#define GPIO19_FSEL		GPIO_GPFSEL1		
#define GPIO19_FBIT 	27
#define GPIO19 			19

// Bank 2
#define GPIO20_FSEL		GPIO_GPFSEL2
#define GPIO20_FBIT 	0
#define GPIO20 	 		20

#define GPIO21_FSEL		GPIO_GPFSEL2	
#define GPIO21_FBIT 	3
#define GPIO21 	 		21

#define GPIO22_FSEL		GPIO_GPFSEL2	
#define GPIO22_FBIT 	6
#define GPIO22 	 		22

#define GPIO23_FSEL		GPIO_GPFSEL2		
#define GPIO23_FBIT 	9
#define GPIO23 			23

#define GPIO24_FSEL		GPIO_GPFSEL2		
#define GPIO24_FBIT 	12
#define GPIO24 			24

#define GPIO25_FSEL		GPIO_GPFSEL2		
#define GPIO25_FBIT 	15
#define GPIO25 			25

#define GPIO26_FSEL		GPIO_GPFSEL2		
#define GPIO26_FBIT 	18
#define GPIO26 			26

#define GPIO27_FSEL		GPIO_GPFSEL2		
#define GPIO27_FBIT 	21
#define GPIO27 			27

#define GPIO28_FSEL		GPIO_GPFSEL2		
#define GPIO28_FBIT 	24
#define GPIO28 			28

#define GPIO29_FSEL		GPIO_GPFSEL2		
#define GPIO29_FBIT 	27
#define GPIO29 			29

// Bank 3
#define GPIO30_FSEL		GPIO_GPFSEL3
#define GPIO30_FBIT 	0
#define GPIO30 	 		30

#define GPIO31_FSEL		GPIO_GPFSEL3	
#define GPIO31_FBIT 	3
#define GPIO31 	 		31

#define GPIO32_FSEL		GPIO_GPFSEL3	
#define GPIO32_FBIT 	6
#define GPIO32 	 		0

#define GPIO33_FSEL		GPIO_GPFSEL3		
#define GPIO33_FBIT 	9
#define GPIO33 			1

#define GPIO34_FSEL		GPIO_GPFSEL3		
#define GPIO34_FBIT 	12
#define GPIO34 			2

#define GPIO35_FSEL		GPIO_GPFSEL3		
#define GPIO35_FBIT 	15
#define GPIO35 			3

#define GPIO36_FSEL		GPIO_GPFSEL3		
#define GPIO36_FBIT 	18
#define GPIO36 			4

#define GPIO37_FSEL		GPIO_GPFSEL3		
#define GPIO37_FBIT 	21
#define GPIO37 			5

#define GPIO38_FSEL		GPIO_GPFSEL3		
#define GPIO38_FBIT 	24
#define GPIO38 			6

#define GPIO39_FSEL		GPIO_GPFSEL3		
#define GPIO39_FBIT 	27
#define GPIO39 			7

// Bank 4
#define GPIO40_FSEL		GPIO_GPFSEL4
#define GPIO40_FBIT 	0
#define GPIO40 	 		8

#define GPIO41_FSEL		GPIO_GPFSEL4	
#define GPIO41_FBIT 	3
#define GPIO41 	 		9

#define GPIO42_FSEL		GPIO_GPFSEL4	
#define GPIO42_FBIT 	6
#define GPIO42 	 		10

#define GPIO43_FSEL		GPIO_GPFSEL4		
#define GPIO43_FBIT 	9
#define GPIO43 			11

#define GPIO44_FSEL		GPIO_GPFSEL4		
#define GPIO44_FBIT 	12
#define GPIO44 			12

#define GPIO45_FSEL		GPIO_GPFSEL4		
#define GPIO45_FBIT 	15
#define GPIO45 			13

#define GPIO46_FSEL		GPIO_GPFSEL4		
#define GPIO46_FBIT 	18
#define GPIO46 			14

#define GPIO47_FSEL		GPIO_GPFSEL4		
#define GPIO47_FBIT 	21
#define GPIO47 			15

#define GPIO48_FSEL		GPIO_GPFSEL4		
#define GPIO48_FBIT 	24
#define GPIO48 			16

#define GPIO49_FSEL		GPIO_GPFSEL4		
#define GPIO49_FBIT 	27
#define GPIO49 			17

// Bank 5
#define GPIO50_FSEL		GPIO_GPFSEL5
#define GPIO50_FBIT 	0
#define GPIO50 	 		18

#define GPIO51_FSEL		GPIO_GPFSEL5	
#define GPIO51_FBIT 	3
#define GPIO51 	 		19

#define GPIO52_FSEL		GPIO_GPFSEL5	
#define GPIO52_FBIT 	6
#define GPIO52 	 		20

#define GPIO53_FSEL		GPIO_GPFSEL5		
#define GPIO53_FBIT 	9
#define GPIO53 			21

uint32_t *_get_gpio_reg();
void _set_gpio_pull(uint8_t bank, uint8_t pin, uint8_t dir);

#endif
