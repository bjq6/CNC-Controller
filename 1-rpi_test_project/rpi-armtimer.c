
#include <stdint.h>
#include "rpi-armtimer.h"

static rpi_arm_timer_t* rpiArmTimer = (rpi_arm_timer_t*)RPI_ARMTIMER_BASE;

rpi_arm_timer_t* _get_arm_timer(void) {
    return rpiArmTimer;
}
