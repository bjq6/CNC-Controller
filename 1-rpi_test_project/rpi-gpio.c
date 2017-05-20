#include <stdint.h>

#include "rpi-gpio.h"
#include "rpi-systimer.h"

volatile uint32_t* gpio = (uint32_t*)GPIO_BASE;

uint32_t *_get_gpio_reg() {
	return (uint32_t *)gpio;
}

void _set_gpio_pull(uint8_t pin, uint8_t dir) {
	int clk = 0;
	if (pin >= 32) {
		clk = GPIO_GPPUDCLK1;
	} else {
		clk = GPIO_GPPUDCLK0;
	}

	gpio[GPIO_GPPUD] = dir;
    waitcnt32(CNT32() + 150);
    gpio[clk] |= (1 << pin);
    waitcnt32(CNT32() + 150);
    gpio[GPIO_GPPUD] = 0;
    gpio[clk] &= ~(1 << pin);
}