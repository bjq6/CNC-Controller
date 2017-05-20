#include <stdint.h>

#include "rpi-pwm.h"

static rpi_pwm_t* pwm_ctrl = (rpi_pwm_t*)RPI_PWM_BASE;

rpi_pwm_t *_get_pwm_ctrl() {
	return pwm_ctrl;
}