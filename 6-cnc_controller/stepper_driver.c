#include "stepper_driver.h"

#include <stdlib.h>
#include <math.h>

#include "bcm/bcm-includes.h"

extern volatile uint32_t *gpio;
extern volatile rpi_irq_controller_t *irq_ctrl;
extern volatile rpi_arm_timer_t *timer;

volatile uint32_t step_en = 0;

axis_t x_axis = {0};
axis_t y_axis = {0};
axis_t z_axis = {0};

uint32_t sd_state = SD_NOTREADY;

void setup_vars();
void setup_pins();
uint32_t axis_active(axis_t* a);

/*
 * Main function
 */ 
void sd_main() {
	setup_pins();
	setup_vars();

	PWR_LED_ON();
	
	irq_ctrl->enable_IRQ_basic |= RPI_BASIC_ARM_TIMER_IRQ;
	step_en = 1;

	run_homing(&x_axis);
	run_homing(&y_axis);

	sd_state = SD_READY;

	uint32_t loop_t;

	while(1) {
		loop_t = CNT32();
		update_pid(&x_axis);
		update_pid(&y_axis);

		waitcnt32(loop_t + CLKFREQ/1000); //1 ms update loop
    }
}

/*
 * Setup all of the IO pins
 */
void setup_pins() {
	gpio[GPIO13_FSEL] &= ~(7 << GPIO13_FBIT);
	gpio[GPIO13_FSEL] |= (1 << GPIO13_FBIT);
	gpio[GPIO_GPCLR0] |= 1 << GPIO13;

	// ==== X_AXIS =====
	// setup step pin
	x_axis.step_pin = GPIO19;
	x_axis.step_fsel = GPIO19_FSEL;
	x_axis.step_fbit = GPIO19_FBIT;
	x_axis.step_set = GPIO_GPSET0;
	x_axis.step_clr	= GPIO_GPCLR0;

	gpio[x_axis.step_fsel] &= ~(7 << x_axis.step_fbit);
	gpio[x_axis.step_fsel] |= 1 << x_axis.step_fbit;

	// setup direction pin
	x_axis.dir_pin = GPIO26;
	x_axis.dir_fsel = GPIO26_FSEL;
	x_axis.dir_fbit = GPIO26_FBIT;
	x_axis.dir_set = GPIO_GPSET0;
	x_axis.dir_clr = GPIO_GPCLR0;

	gpio[x_axis.dir_fsel] &= ~(7 << x_axis.dir_fbit);
	gpio[x_axis.dir_fsel] |= 1 << x_axis.dir_fbit;

	// setup encoder pin
	x_axis.enc.enc_pin = GPIO6;
	x_axis.enc.enc_fsel = GPIO6_FSEL;
	x_axis.enc.enc_fbit = GPIO6_FBIT;
	x_axis.enc.enc_lev = GPIO_GPLEV0;
	
	gpio[x_axis.enc.enc_fsel] &= ~(7 << x_axis.enc.enc_fbit);
	_set_gpio_pull(GPIO_BANK0, x_axis.enc.enc_pin, GPIO_PUP);

	// setup negative limit pin
	x_axis.nlim_pin = GPIO2;
	x_axis.nlim_fsel = GPIO2_FSEL;
	x_axis.nlim_fbit = GPIO2_FBIT;
	x_axis.nlim_lev = GPIO_GPLEV0;
	
	gpio[x_axis.nlim_fsel] &= ~(7 << x_axis.nlim_fbit);
	_set_gpio_pull(GPIO_BANK0, x_axis.nlim_pin, GPIO_PUP);

	
	// ==== Y_AXIS =====
	// setup step pin
	y_axis.step_pin = GPIO20;
	y_axis.step_fsel = GPIO20_FSEL;
	y_axis.step_fbit = GPIO20_FBIT;
	y_axis.step_set = GPIO_GPSET0;
	y_axis.step_clr	= GPIO_GPCLR0;

	gpio[y_axis.step_fsel] &= ~(7 << y_axis.step_fbit);
	gpio[y_axis.step_fsel] |= 1 << y_axis.step_fbit;

	// setup direction pin
	y_axis.dir_pin = GPIO21;
	y_axis.dir_fsel = GPIO21_FSEL;
	y_axis.dir_fbit = GPIO21_FBIT;
	y_axis.dir_set = GPIO_GPSET0;
	y_axis.dir_clr = GPIO_GPCLR0;

	gpio[y_axis.dir_fsel] &= ~(7 << y_axis.dir_fbit);
	gpio[y_axis.dir_fsel] |= 1 << y_axis.dir_fbit;

	// setup encoder pin
	y_axis.enc.enc_pin = GPIO5;
	y_axis.enc.enc_fsel = GPIO5_FSEL;
	y_axis.enc.enc_fbit = GPIO5_FBIT;
	y_axis.enc.enc_lev = GPIO_GPLEV0;
	
	gpio[y_axis.enc.enc_fsel] &= ~(7 << y_axis.enc.enc_fbit);
	_set_gpio_pull(GPIO_BANK0, y_axis.enc.enc_pin, GPIO_PUP);

	// setup negative limit pin
	y_axis.nlim_pin = GPIO3;
	y_axis.nlim_fsel = GPIO3_FSEL;
	y_axis.nlim_fbit = GPIO3_FBIT;
	y_axis.nlim_lev = GPIO_GPLEV0;
	
	gpio[y_axis.nlim_fsel] &= ~(7 << y_axis.nlim_fbit);
	_set_gpio_pull(GPIO_BANK0, y_axis.nlim_pin, GPIO_PUP);
	
}

/* 
 * IRQ for time step: send pulses here. 
 */

void sd_IRQ() {
	if (!step_en) {
		PWR_LED_OFF();
		return;
	}

	if (x_axis.step_div) {
		x_axis.step_cntr++;
		ACT_LED_ON();

		uint32_t step_div_mag = abs(x_axis.step_div);
		if (x_axis.step_cntr >= step_div_mag) {
			if (x_axis.step_div > 0) {
				set_dir(&x_axis, 1);
				x_axis.step_pos++;
			} else {
				set_dir(&x_axis, 0);
				x_axis.step_pos--;
			}

			step(&x_axis);
			x_axis.step_cntr = 0;
		}
	} else {
		ACT_LED_OFF();
	}

	if (y_axis.step_div) {

		y_axis.step_cntr++;
		ACT_LED_ON();

		uint32_t step_div_mag = abs(y_axis.step_div);
		if (y_axis.step_cntr >= step_div_mag) {
			if (y_axis.step_div > 0) {
				set_dir(&y_axis, 1);
				y_axis.step_pos++;
			} else {
				set_dir(&y_axis, 0);
				y_axis.step_pos--;
			}

			step(&y_axis);
			y_axis.step_cntr = 0;
		}
	} else {
		ACT_LED_OFF();
	}

}

/* 
 * Generate a step pulse for a given axis a
 */
void step(axis_t *a) {

	gpio[a->step_set] = 1 << a->step_pin;
    waitcnt32(CNT32() + 1);
    gpio[a->step_clr] = 1 << a->step_pin;
}

/*
 * Generate the direction signal for the given axis a
 */
void set_dir(axis_t *a, uint8_t dir) {
	if (dir) {
		gpio[a->dir_set] = 1 << a->dir_pin;
	} else {
		gpio[a->dir_clr] = 1 << a->dir_pin;
	}
}

/* 
 * Run this homing procedure. This will run the motors in the negative direction until they hit a limit switch. 
 * At that point, motion stops and the encoder offsets for the home position are recorded.
 */
void run_homing(axis_t *a) {
	sd_state = SD_HOMING;

	printf("Start homing axis %d\n", (int)a->axis_id);

	volatile uint32_t *lev = &(gpio[a->nlim_lev]);
	uint8_t pin = 1 << a->nlim_pin;

	a->step_div = -20;

	a->enc.home_offset = 0;
	while ((*lev & pin));
	a->step_div = 0;
 
	read_enc(a);
	a->enc.home_offset = a->enc.a;
	a->enc.r = 0;
	a->enc.a_abs = 0;
	read_enc(a);
	a->target = 0;
	a->target_a = 0;
	printf("Axis %d offset is %f\n", (int)a->axis_id, a->enc.home_offset);
}

/*
 * Sets the target position for an axis. The PID loop will then move the stage to the given position at the given speed
 */
void set_target_axis(axis_t *a, float t, float v_inv) {
	a->target = t;
	a->target_a = 2*M_PI*t/a->mm_per_rev;
	a->speed_inv = v_inv;
}

/*
 * Set the target position for the stage and the feed rate to use to get there.
 */
void set_target(float x, float y, float f) {
	float dx = x - x_axis.pos;
	float dy = y - y_axis.pos;
	float r = sqrt(dx*dx + dy*dy);

	float vx = f*dx/r;
	float vy = f*dy/r;

	int vx_inv = abs(10/vx);
	int vy_inv = abs(10/vy);

	set_target_axis(&x_axis, x, vx_inv);
	set_target_axis(&y_axis, y, vy_inv);

	//printf("%d, %d\n", vx_inv, vy_inv);
}

/*
 * Get the various objects and states
 */
axis_t* get_x_axis() {
	return &x_axis;
}

axis_t* get_y_axis() {
	return &y_axis;
}

axis_t* get_z_axis() {
	return &z_axis;
}

uint32_t get_sd_state() {
	return sd_state;
}

/*
 * returns true if an axis is more than MOTION_EPS away from the target.
 * True implies the axis is atively moving to get to it's destination.
 */
uint32_t axis_active(axis_t* a) {
	if (fabs(a->pos - a->target) > MOTION_EPS) {
		return 1;
	} else {
		return 0;
	}
}

/*
 * Returns true if any axis is active
 */
uint32_t motion_active() {
	return axis_active(&x_axis) + axis_active(&y_axis);
}

/*
 * setup variables
 */
void setup_vars() {
	x_axis.pid.Kp = 0.5;
	x_axis.pid.Kd = 1;
	x_axis.pid.Ki = 1;
	x_axis.step_target = 0;
	x_axis.step_pos = 0;
	x_axis.enc.r = 0;
	x_axis.enc.a = 0;
	x_axis.enc.a_abs = 0;
	x_axis.speed_inv = 10000;
	x_axis.step_div = 0;
	x_axis.step_cntr = 0;
	x_axis.axis_id = X_AXIS;

	x_axis.mm_per_rev = 8.0;
	set_dir(&x_axis, 1);

	y_axis.pid.Kp = 0.5;
	y_axis.pid.Kd = 1;
	y_axis.pid.Ki = 1;
	y_axis.step_target = 0;
	y_axis.step_pos = 0;
	y_axis.enc.r = 0;
	y_axis.enc.a = 0;
	y_axis.enc.a_abs = 0;
	y_axis.speed_inv = 10000;
	y_axis.step_div = 0;
	y_axis.step_cntr = 0;
	y_axis.axis_id = Y_AXIS;

	y_axis.mm_per_rev = 8.0;
	set_dir(&y_axis, 1);

}

/* 
 * Calculate PID state variables and update the motor velocity for a given axis.
 */
void update_pid(axis_t *a) {
	read_enc(a);

	float e = (a->enc.a_abs - a->target_a);
	a->pid.de = e - a->pid.de;
	a->pid.ie += e;
	a->pid.e = e;

	a->step_div = -(int)(a->pid.Kp/a->pid.e + a->pid.Kd/a->pid.de + a->pid.Ki/a->pid.ie)*a->speed_inv;


	if (a->step_div == 0) {
		if (a->pid.e < 0) {
			a->step_div = a->speed_inv;
		} else {
			a->step_div = -a->speed_inv;
		}
	}

	if (a->step_div > 1000 || a->step_div < -1000) {
		a->step_div = 0;
	}
}

/* 
 * Read the pulse width generated by the encoder and convert it to rotation angle, in radians.
 * Also detect overflow and underflow to counte revolutions
 */
void read_enc(axis_t *a) {

	uint32_t t0 = 0;
	uint32_t t1 = 0;
	const int32_t wmin = 32;
	const int32_t wmax = 377;

	float prev_x = a->enc.a;
	float prev_x_abs = a->enc.a_abs;

	volatile uint32_t *xlev = &(gpio[a->enc.enc_lev]);
	uint8_t xpin = 1 << a->enc.enc_pin;

	while (*xlev & xpin); // wait for pin to go low
	while (!(*xlev & xpin));
	t0 = CNT32();
	while (*xlev & xpin);
	t1 = CNT32();

	int32_t w = t1-t0-wmin; // get the high width

	double an = 2*M_PI*w/(double)wmax;  // calculate the angle in radians
	//float da = a - x_axis.enc.a; // difference between current and last read.
	if (an > 2*M_PI) an = 2*M_PI;
	if (an < 0) an = 0;

	a->enc.a = an;

	if (an > 4.5 && prev_x < 1.78) { // we overflowed, add a revolution.
		a->enc.r -= 1;
	} else if (an < 1.78 && prev_x > 4.5) { // we underflowed, sub a revolution. 
		a->enc.r += 1;
	}

	a->enc.a_abs = 2*M_PI*a->enc.r + an - a->enc.home_offset;
	a->enc.a_abs = (a->enc.a_abs + prev_x_abs)/2;

	a->pos = a->mm_per_rev*a->enc.a_abs/(2*M_PI);

}