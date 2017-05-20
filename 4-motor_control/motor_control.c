#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "bcm/bcm-includes.h"

#include "stepper_driver.h"

volatile uint32_t *gpio;
volatile rpi_arm_timer_t *timer;
volatile rpi_irq_controller_t *irq_ctrl;
volatile aux_t *aux;

IRQ() {
    if (timer->maskedIRQ == 1) { // timer ISR 
        sd_IRQ();
    } else if ((aux->MU_IIR & 0x6) == 4) { // UART ISR
        uint8_t c = _uart_rx(); // get it out of the buffer to clear the interrupt
        _uart_tx(c);
    }

    timer->IRQclear = 1;
}

HANG() {

    _uart_tx('-');
    _uart_tx('e');
    _uart_tx(48+get_core_id());
    _uart_tx('-');
    _uart_tx('\n');
    
    while(1) {
        ACT_LED_ON();
        PWR_LED_OFF();
        waitcnt32(CNT32() + CLKFREQ/5);
        ACT_LED_OFF();
        PWR_LED_ON();
        waitcnt32(CNT32() + CLKFREQ/5);
    }
}

void _init() {

    _disable_irq();

    // get the peripheral objects
    gpio = _get_gpio_reg();
    timer = _get_arm_timer();
    irq_ctrl = _get_irq_controller();
    aux = _get_aux();

    // set power and activity leds as outputs
    gpio[ACT_LED_GPFSEL] &= ~(7 << ACT_LED_GPFBIT);
    gpio[ACT_LED_GPFSEL] |= (1 << ACT_LED_GPFBIT);
    gpio[PWR_LED_GPFSEL] &= ~(7 << PWR_LED_GPFBIT);
    gpio[PWR_LED_GPFSEL] |= (1 << PWR_LED_GPFBIT);

    _uart_init(115200, 8, AUX_MU_RX_IRQ_ENABLE);

    // load the number of counts between interrupts. 
    timer->load = SD_T_STEP;

    // set up the arm timer. 23 bits, enable it, enable interrupt generation, prescaler of 1.
    timer->control = 
            RPI_ARMTIMER_CTRL_23BIT |
            RPI_ARMTIMER_CTRL_ENABLE |
            RPI_ARMTIMER_CTRL_INT_ENABLE |
            RPI_ARMTIMER_CTRL_PRESCALE_1;
    
    // enable IRQ to trigger on UART rx
    irq_ctrl->enable_IRQ_1 |= (1 << 29);

    _enable_irq();

    printf("\n-----------------\n");
}

void __attribute__ ((naked)) pp_main() {
    _init_sp((unsigned int *)CORE2_STACK);
    _init_core();

    while(1);
}

void __attribute__ ((naked)) mc_main() {
    _init_sp((unsigned int *)CORE1_STACK);
    _init_core();

    //PWR_LED_ON();
    while(get_sd_state() != SD_READY);

    printf("Starting motion controller\n");

    axis_t *x = get_x_axis();
    axis_t *y = get_y_axis();

    set_target_axis(x, 100, 20);
    set_target_axis(y, 10, 20);

    while(motion_active());
    printf("At starting position (%f, %f)\n", x->pos, y->pos);

    //uint32_t loop_t = 0;
    
    while(1) {
        //loop_t = CNT32();
        set_target(170, 90, 0.1);
        //set_target_axis(x, 170, 8);
        //set_target_axis(y, 90, 8);
        while(motion_active());
        printf("arrived at position (%f, %f)\n", x->pos, y->pos);
        waitcnt32(CNT32() + CLKFREQ);

        set_target(100, 10, 0.1);
        //set_target_axis(x, 100, 8);
        //set_target_axis(y, 10, 8);
        while(motion_active());
        printf("arrived at position (%f, %f)\n", x->pos, y->pos);
        waitcnt32(CNT32() + CLKFREQ);
    
    }
}

void __attribute__ ((naked)) kernel_main() {
    _init();

    ACT_LED_ON();

    start_core(mc_main, CORE1_ADR);

    sd_main();

}