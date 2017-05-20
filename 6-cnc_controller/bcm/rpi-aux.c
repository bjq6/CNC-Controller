
#include "rpi-aux.h"
#include "rpi-base.h"
#include "rpi-gpio.h"

static aux_t* auxillary = (aux_t*)AUX_BASE;


aux_t* _get_aux(void) {
    return auxillary;
}

/* Define the system clock frequency in Hz for the baud rate calculation.
   This is clearly defined on the BCM2835 datasheet errata page:
   http://elinux.org/BCM2835_datasheet_errata */
#define SYS_FREQ    250000000

void _uart_init(int baud, int bits, int irq) {

    /* As this is a mini uart the configuration is complete! Now just
       enable the uart. Note from the documentation in section 2.1.1 of
       the ARM peripherals manual:

       If the enable bits are clear you will have no access to a
       peripheral. You can not even read or write the registers */
    auxillary->ENABLES = AUX_ENA_MINIUART;

    /* Disable interrupts for now */
    //auxillary->IRQ &= ~AUX_IRQ_MU;

    auxillary->MU_IER = 0;

    /* Disable flow control,enable transmitter and receiver! */
    auxillary->MU_CNTL = 0;

    /* Decide between seven or eight-bit mode */
    if( bits == 8 )
        auxillary->MU_LCR = AUX_MULCR_8BIT_MODE;
    else
        auxillary->MU_LCR = 0;

    auxillary->MU_MCR = 0;

    /* Disable all interrupts from MU and clear the fifos */
    auxillary->MU_IER = irq;

    auxillary->MU_IIR = 0xC6;

    /* Transposed calculation from Section 2.2.1 of the ARM peripherals
       manual */
    auxillary->MU_BAUD = (SYS_FREQ/(8*baud)) - 1;

     /* Setup GPIO 14 and 15 as alternative function 5 which is
        UART 1 TXD/RXD. These need to be set before enabling the UART */
    _get_gpio_reg()[GPIO14_FSEL] &= ~(7 << GPIO14_FBIT);
    _get_gpio_reg()[GPIO15_FSEL] &= ~(7 << GPIO15_FBIT);

    _get_gpio_reg()[GPIO14_FSEL] |= (FS_ALT5 << GPIO14_FBIT);
    _get_gpio_reg()[GPIO15_FSEL] |= (FS_ALT5 << GPIO15_FBIT);

    _set_gpio_pull(GPIO_BANK0, GPIO14, GPIO_POFF);
    _set_gpio_pull(GPIO_BANK0, GPIO15, GPIO_POFF);

    /* Disable flow control,enable transmitter and receiver! */
    auxillary->MU_CNTL = AUX_MUCNTL_TX_ENABLE | AUX_MUCNTL_RX_ENABLE;
}


void _uart_tx(char c) {
    /* Wait until the UART has an empty space in the FIFO */
    while( ( auxillary->MU_LSR & AUX_MULSR_TX_EMPTY ) == 0 ) { }

    /* Write the character to the FIFO for transmission */
    auxillary->MU_IO = c;
}

uint8_t _uart_check() {
    return ((auxillary->MU_LSR & 1) == 1);
}

uint8_t _uart_rx() {
    if (_uart_check()) {
        return (auxillary->MU_IO & 0xFF);
    } else {
        return 0;
    }
}
