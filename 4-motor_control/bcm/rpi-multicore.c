#include <stdint.h>

#include "rpi-multicore.h"
#include "rpi-base.h"

uint32_t get_core_number(void) {
  uint32_t core_num;
  asm volatile ("mrc p15, 0, %0, c0, c0, 5" : "=r" (core_num));

  return (core_num & CORE_MASK);
}

void start_core(void (*func)(void), uint32_t adr) {
	PUT32(adr, (uint32_t)func);
}