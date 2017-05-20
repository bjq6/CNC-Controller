#include <stdint.h>

#include "bcm/rpi-systimer.h"
#include "bcm/rpi-gpio.h"
#include "bcm/rpi-base.h"
#include "bcm/rpi-armtimer.h"
#include "bcm/rpi-interrupts.h"
#include "bcm/rpi-aux.h"
#include "bcm/rpi-pwm.h"
#include "bcm/printf.h"

volatile uint32_t *gpio;
volatile rpi_arm_timer_t *timer;
volatile rpi_irq_controller_t *irq_ctrl;
volatile aux_t *aux;
volatile rpi_pwm_t *pwm_ctrl;

IRQ() {
    static int on;
    
    if (timer->maskedIRQ == 1) { // timer ISR -- create a heartbeat. 
        if (on) {
            on = 0;
            PWR_LED_OFF();
        } else {
            on = 1;
            PWR_LED_ON();
        }
    } else if ((aux->MU_IIR & 0x6) == 4) { // UART ISR
        _uart_rx(); // get it out of the buffer to clear the interrupt
    }

    timer->IRQclear = 1;

}

void _init() {
    _disable_irq();

    // get the peripheral objects
    gpio = _get_gpio_reg();
    timer = _get_arm_timer();
    irq_ctrl = _get_irq_controller();
    aux = _get_aux();
    pwm_ctrl = _get_pwm_ctrl();

    // set power and activity leds as outputs
    gpio[ACT_LED_GPFSEL] |= (1 << ACT_LED_GPFBIT);
    gpio[PWR_LED_GPFSEL] |= (1 << PWR_LED_GPFBIT);
    ACT_LED_ON();

    _uart_init(115200, 8, AUX_MU_RX_IRQ_ENABLE);
    init_printf(0, _uart_tx);

    // load the number of counts between interrupts. 
    timer->load = CLKFREQ;

    // set up the arm timer. 23 bits, enable it, enable interrupt generation, prescaler of 1.
    timer->control = 
            RPI_ARMTIMER_CTRL_23BIT |
            RPI_ARMTIMER_CTRL_ENABLE |
            RPI_ARMTIMER_CTRL_INT_ENABLE |
            RPI_ARMTIMER_CTRL_PRESCALE_1;
    
    // enable IRQ to trigger on timer
    irq_ctrl->enable_IRQ_basic |= RPI_BASIC_ARM_TIMER_IRQ;
    // enable IRQ to trigger on UART rx
    irq_ctrl->enable_IRQ_1 |= (1 << 29);

    _enable_irq();

    gpio[GPIO12_FSEL] &= ~(7 << GPIO12_FBIT);
    gpio[GPIO12_FSEL] |= FS_ALT0 << GPIO12_FBIT;

    pwm_ctrl->rng1 = CLKFREQ;
    pwm_ctrl->dat1 = CLKFREQ/2;
    pwm_ctrl->ctl = 1 | (1 << 3) | (1 << 7);

    ACT_LED_OFF();

    printf("Initialization Complete\n");
}

int kernel_main(void) {
    _init(); 
    
    while(1){
        ACT_LED_ON();
        //gpio[GPIO_GPSET0] |= 1 << GPIO12;
        waitcnt32(CNT32() + CLKFREQ/2);
        ACT_LED_OFF();
        //gpio[GPIO_GPCLR0] |= 1 << GPIO12;
        waitcnt32(CNT32() + CLKFREQ/2);
    }
}