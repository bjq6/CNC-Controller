#ifndef RPI_MULTICORE_H
#define RPI_MULTICORE_H

#define CORE_MASK 3

#define CORE0 0
#define CORE1 1
#define CORE2 2
#define CORE3 3

#define CORE1_STACK 0x7C00000
#define CORE2_STACK 0x7800000
#define CORE3_STACK 0x7400000

#define CORE1_ADR 0x4000009C
#define CORE2_ADR 0x400000AC
#define CORE3_ADR 0x400000BC

uint32_t get_core_number(void);
void start_core(void (*func)(void), uint32_t adr);

#endif