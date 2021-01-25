// ARM Cortex M4 GTPM example with interrupt triggers

// An interrupt is triggered by the 0A timer module.
// Attention: both match and timeout events generate interrupt requests!

// The corresponding ISR toggles PD0 pin.
// High phase 200ms (pin high between timeout event and match event),
// low phase 300ms (pin low between the match event and the timeout event).


#include <stdint.h>
#include "inc/tm4c1294ncpdt.h"

volatile int cycleCount = 10;

// ISR of Timer 0A
void timer0AISR(void)
{

    // figure out which event caused the interrupt
    if (TIMER0_MIS_R & 0x01) {
        // timeout interrupt

        // clear the corresponding event bit
        TIMER0_ICR_R = 0x01;

        // turn on the LED (high phase)
        GPIO_PORTD_AHB_DATA_R = 0x1;
    } else if (TIMER0_MIS_R & 0x10) {
        // match interrupt

        // clear the corresponding event bit
        TIMER0_ICR_R = 0x10;

        //turn off the LED (low phase)
        GPIO_PORTD_AHB_DATA_R = 0x0;

        --cycleCount;
    }

}


/**
 * main.c
 */
int main(void)
{

    // initialization

    // configure GPIO Port D
    SYSCTL_RCGCGPIO_R = 0x00000008;             // clock port D
    while(!(SYSCTL_PRGPIO_R & 0x00000008));     // wait for port D activation
    GPIO_PORTD_AHB_DEN_R  |= 0x01;              // PD(0) enable
    GPIO_PORTD_AHB_DIR_R  |= 0x01;              // PD(0) output
    GPIO_PORTD_AHB_DATA_R |= 0x01;              // PD(0) is initially on

    // configure Timer 0
    SYSCTL_RCGCTIMER_R |= (1<<0);               // system clock  auf Timer 0
    while(!(SYSCTL_PRTIMER_R & 0x01));          // wait for Timer 0 activation
    TIMER0_CTL_R     &= ~0x01;                  // disable Timer 0
    TIMER0_CFG_R      = 0x04;                   // 2 x 16-bit mode
    TIMER0_TAMR_R     = 0x22;                   // periodic mode + match enable
    TIMER0_TAPR_R     = 123-1;                  // prescaler PR = ceil(16M/2^16*0.5)
    TIMER0_TAILR_R    = 65041-1;                // ILR = ceil(16M/123*0.5)
    TIMER0_TAMATCHR_R = 39024-1;                // MV = ceil(16M/123*0.3)
    TIMER0_IMR_R      = 0x11;                   // enable timeout and match interrupt requests from Timer A
    TIMER0_ICR_R      = 0x11;                   // clear corresponding event bits
    TIMER0_CTL_R     |= 0x03;                   // enable Timer 0

    NVIC_EN0_R |= 1<<19;                        // enable 16/32-Bit Timer 0A IRQ in NVIC (IRQ 19)

    // only 10 cycles
    while(cycleCount > 0) {
    }

    NVIC_DIS0_R = 1<<19;                        // disable the interrupt

	return 0;

}
