#include <stdint.h>
#include "rpi-systimer.h"

static rpi_sys_timer_t* rpiSystemTimer = (rpi_sys_timer_t*)RPI_SYSTIMER_BASE;

rpi_sys_timer_t* _get_sys_timer(void) {
    return rpiSystemTimer;
}

uint32_t CNT32(void) {
    return rpiSystemTimer->counter_lo;
}

uint64_t CNT64(void) {
	return rpiSystemTimer->counter_lo + ((uint64_t)rpiSystemTimer->counter_hi << 32);
}

void waitcnt32( uint32_t cnt ) {
    while(rpiSystemTimer->counter_lo < cnt);
}
