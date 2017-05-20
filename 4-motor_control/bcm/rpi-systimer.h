
#ifndef RPI_SYSTIMER_H
#define RPI_SYSTIMER_H

#include <stdint.h>

#include "rpi-base.h"

#define RPI_SYSTIMER_BASE	(PERIPHERAL_BASE + 0x3000)

#define CLKFREQ 1000000UL

typedef struct {
    volatile uint32_t control_status;
    volatile uint32_t counter_lo;
    volatile uint32_t counter_hi;
    volatile uint32_t compare0;
    volatile uint32_t compare1;
    volatile uint32_t compare2;
    volatile uint32_t compare3;
} rpi_sys_timer_t;

rpi_sys_timer_t* _get_sys_timer(void);
uint32_t CNT32(void);
uint64_t CNT64(void);
void waitcnt32(uint32_t cnt);

#endif
