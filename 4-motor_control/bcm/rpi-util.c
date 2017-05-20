#include "rpi-base.h"

volatile mutex_t printf_mutex = 0;

uint32_t get_core_id(void) {
    uint32_t core_id;
    asm volatile ("mrc p15, 0, %0, c0, c0,  5" : "=r" (core_id));
    return core_id & 0x3;
}

volatile mutex_t *get_pf_m() {
	return &printf_mutex;
}