#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "interpreter/parser.h"
#include "interpreter/nuts_bolts.h"
#include "interpreter/queue.h"
#include "bcm/bcm-includes.h"
#include "stepper_driver.h"
#include "gcode_demos.h"

#define SPEED_DIV 2.5

volatile uint32_t *gpio;
volatile rpi_arm_timer_t *timer;
volatile rpi_irq_controller_t *irq_ctrl;
volatile aux_t *aux;

volatile uint8_t uart_buffer[80] = {0};
volatile uint8_t buf_ptr = 0;

volatile int reset;
volatile int cycle_start;
volatile int demo;

#define F_RAPID 2.5

/*
 * Different commands for the serial interface 
 */
enum {
    CMD_NONE = 0,
    CMD_DEMO,
    CMD_CYCLE_START,
    CMD_RESET,
};

/*
 * IRQ Interrupt handler
 */
IRQ() {
    if (timer->maskedIRQ == 1) { // timer ISR 
        sd_IRQ();
    } else if ((aux->MU_IIR & 0x6) == 4) { // UART ISR
        uint8_t c = _uart_rx(); // get it out of the buffer to clear the interrupt
        uart_buffer[buf_ptr++] = c;
        _uart_tx(c);
    }

    timer->IRQclear = 1;
}

/*
 * Hang (alarm) interrupt handler
 */
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

/* 
 * High level initialization function. Does the following: 
 * 
 *  1. Sets up GPIO for the LEDs
 *  2. configures UART interface
 *  3. Set up the ARM timer to generate a step pulse clock
 *  4. Enable required interrupts in the interrupt controller
 *  5. Print "-----------------" to confirm everything is working on the serial consol
 */
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
    buf_ptr = 0;

    // load the number of counts between interrupts. 
    timer->load = SD_T_STEP;

    // set up the arm timer. 23 bits, enable it, enable interrupt generation, prescaler of 1.
    timer->control = 
            RPI_ARMTIMER_CTRL_23BIT |
            RPI_ARMTIMER_CTRL_ENABLE |
            RPI_ARMTIMER_CTRL_INT_ENABLE |
            RPI_ARMTIMER_CTRL_PRESCALE_1;
    
    // enable IRQ to trigger on UART rx
    //irq_ctrl->enable_IRQ_1 |= (1 << 29);

    _enable_irq();

    printf("\n-----------------\n");
}

/*
 * Parse cmd for a proper command. If the command requires a value, it will be stores in *val.
 */
uint8_t process_cmd(char *cmd, uint8_t *val) {
    if (cmd[0] != '$') return CMD_NONE;

    switch (cmd[1]) {
        case 'c':
            return CMD_CYCLE_START;
            break;
        case 'r':
            return CMD_RESET;
            break;
        case 'd':
            *val = cmd[2]-48;
            return CMD_DEMO;
            break;
        default:
            return CMD_NONE;
    }
}

/* 
 * Main function for the serial interface. The main loop will:
 * 
 *  1. Read in UART characters into a buffer until a '\n' is recieved 
 *  2. Process the input text
 *  3. If its a valid command, set the appropriate flag. 
 */
void iface_main() {
    printf("Starting Interface Core\n");

    buf_ptr = 0;

    while(1) {
        if (_uart_check()) {
            uint8_t c = _uart_rx();
            uart_buffer[buf_ptr++] = c;
            //_uart_tx(c);
            if (c == '\n') {
                uart_buffer[buf_ptr] = 0;
                printf("%s", uart_buffer);
                buf_ptr = 0;
                uint8_t v = 0;
                uint8_t cmd = process_cmd((char*)uart_buffer, &v);

                switch (cmd) {
                    case CMD_CYCLE_START:
                        cycle_start = 1;
                        break;
                    case CMD_RESET:
                        reset = 1;
                        break;
                    case CMD_DEMO:
                        demo = v;
                        break;
                    case CMD_NONE:
                    default:
                        break;
                }
            }
        }
    }
}

/*
 * Main function for the path planner.
 */
void mc_main() {
    reset = 0;
    cycle_start = 0;
    demo = 0;

    // wait for stepper driver to be ready
    while(get_sd_state() != SD_READY);

    printf("Starting path planner\n");
    axis_t *x = get_x_axis();
    axis_t *y = get_y_axis();

    // move away from (0,0) to avoid getting stuck on the axis (this later should be implemented as a G54 command to set an origin offset)
    set_target(10, 10, F_RAPID/SPEED_DIV);
    while(motion_active());
    set_target(10, 10, 0.1);

    while(1) {
        
        // check which demo program is selected
        char *gf = gdemo1; 
        do {
            while (!cycle_start);
            cycle_start = 0;

            switch(demo) {
                case 1:
                    gf = gdemo1;
                    break;
                case 2:
                    gf = gdemo2;
                    break;
                case 3:
                    gf = gdemo3;
                    break;
                default:
                    demo = 0;
                    printf("Please select a demo by entering $d[n]\n");
            }
        } while (demo == 0);

        printf("demo %d\n", (int)demo);
        printf("%d, %d\n", (int)gf, (int)gdemo3);

        printf("Cycle start\n");

        char *gcode_file[80];

        int n=0;
        int i=0;
        int acc=0;
        gcode_file[0] = gf;

        // parse the GCode file into lines
        printf("Program To Run:\n");
        while ( strcmp( gcode_file[n], "eof" ) != 0 ) {
            printf("%s\n", gcode_file[n]); 
            find_first(gcode_file[n], '\0', &i);
            acc+=i+1;
            n++;
            gcode_file[n]=gf+acc;
        }
        printf("\n========\n");

        n = 0;

        queue pos_q;
        queue speed_q;
        init_q(&pos_q);
        init_q(&speed_q);

        vector pos0 = {10, 10, 0};

        printf("At starting position (%f, %f)\n", x->pos, y->pos);

        // loop through the gcode program line by line
        while (strcmp( gcode_file[n], "eof" ) != 0) {
            // Initializations
            int g_code = -1;
            float f_val = 2, r_val = 0;
            vector victor = { 0, 0, 0 };
            vector pos = pos0;
            int cc = -1;

            // Populate Initializations from current GCode command
            read_line((char*)gcode_file[n], &g_code, &f_val, &r_val, &victor);

            // Main GCode switch
            // 0, 1 are linear motion 
            // 2 is clockwise motion,
            // 3 is counter clockwise motion
            switch(g_code){
                case 0: 
                    f_val = F_RAPID;
                case 1: 
                    set_target(victor.x, victor.y, f_val/SPEED_DIV);
                    while(motion_active());
                    break;
                case 2: 
                    cc = 1;
                case 3: {
                    process_circular(&pos_q, &victor, &pos, r_val, cc);
                    while (!isEmpty(&pos_q)) {
                        vector p = removeData(&pos_q);
                        set_target(p.x, p.y, f_val/SPEED_DIV);
                        while(motion_active());
                        //printf("next step = (%f, %f)\n", p.x, p.y);
                    }
                    break;
                }
                default: 
                    printf("Weird... I thought this would have been caught earlier: %d\n", g_code);
            }

            pos0.x = victor.x;
            pos0.y = victor.y;

            printf("%s\n", gcode_file[n]); 
            n++;

            
        }
        printf("Done\n");
        while (!reset);
        reset = 0;

        printf("Reset\n");

        set_target(10, 10, F_RAPID);
        while(motion_active());
        set_target(10, 10, 0.1);
    }
}

/*
 * naked entry function for the path planner. These functions must be "naked" because 
 * so that the compiler doesn't try using the stack to push registers before the stack is setup.
 */ 
void __attribute__ ((naked)) mc_entry() {
    // initialize the stack
    _init_sp((unsigned int *)CORE1_STACK);
    // initialize the core
    _init_core();

    mc_main();
}

/*
 * Naked entry function for the serial interface
 */
void __attribute__ ((naked)) iface_entry() {
    _init_sp((unsigned int *)CORE2_STACK);
    _init_core();

    iface_main();
}

/*
 * naked main function. called from the assembly startup code
 */
void __attribute__ ((naked)) kernel_main() {
    _init();

    ACT_LED_ON();

    // start the secondary cores to run their entry functions
    start_core(mc_entry, CORE1_ADR);
    start_core(iface_entry, CORE2_ADR);
    waitcnt32(CNT32() + CLKFREQ/10);

    // core 0 will run the main stepper driver function
    sd_main();

}