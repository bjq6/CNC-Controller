#ifndef RPI_BASE_H
#define RPI_BASE_H

#include <stdint.h>

#define PERIPHERAL_BASE     0x3F000000UL

/*
 * Functions from Assembly 
 */
typedef volatile uint32_t rpi_reg_rw_t;
typedef volatile const uint32_t rpi_reg_ro_t;
typedef volatile uint32_t rpi_reg_wo_t;

typedef volatile uint64_t rpi_wreg_rw_t;
typedef volatile const uint64_t rpi_wreg_ro_t;

extern void PUT32 (unsigned int, unsigned int);
extern unsigned int GET32 (unsigned int);
extern void _enable_irq (void);
extern void _disable_irq (void);

extern void _init_core();
extern void _init_sp(volatile unsigned int *);

/*
 * Types 
 */

typedef unsigned int mutex_t;


/*
 * Utility Functions
 */

uint32_t get_core_id(void);

volatile mutex_t *get_pf_m(); 

void inline _lock_mutex(volatile mutex_t *m) {
	while (*m == 1);
    *m = 1;
}
void inline _unlock_mutex(volatile mutex_t *m) {
	*m = 0;
}

#define _lock_pf_mutex() _lock_mutex(get_pf_m());
#define _unlock_pf_mutex() _unlock_mutex(get_pf_m());

#endif
