/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"
#include "lcd.h"  // For DEBUGging


volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile states STATE = LOW; // State of ping echo pulse
volatile unsigned long NUM_OVERFLOWS = 0;
// volatile int i = 0;

void ping_init (void){

  // YOUR CODE HERE

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure port B for PB3
    SYSCTL_RCGCGPIO_R |= 0x02;
    while ( (SYSCTL_PRGPIO_R & 0x02) == 0) {}

    // Set PB3 as a digital output
    GPIO_PORTB_DEN_R |= 0x08;
    // Set PB3 as an alternate function
    GPIO_PORTB_AFSEL_R |= 0x08;
    // Set PB3's alternate function
    GPIO_PORTB_PCTL_R &= ~0x0000F000;
    GPIO_PORTB_PCTL_R |= 0x00007000;
    // GPIO_PORTB_AMSEL_R |= 0x08; // Not an analog input/output (?)

    // Configure and enable the timer
    SYSCTL_RCGCTIMER_R |= 0x08;
    while ( (SYSCTL_PRTIMER_R & 0x08) == 0) {}

    // disable timer 3b
    TIMER3_CTL_R &= ~0x0100;

    // Select the 16-bit timer
    TIMER3_CFG_R &= ~0x7;
    TIMER3_CFG_R |= 0x4;
    // Set timer 3b to count down mode
    TIMER3_TBMR_R &= ~0x10;
    // Set timer to edge-time mode, capture mode
    TIMER3_TBMR_R |= 0x7;
    // Set timer 3b to detect events for both rising and falling edges
    TIMER3_CTL_R |= 0x0C00;
    // Set the max/initial timer and prescaler values
    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_TBPR_R |= 0xFF;
    // Set the timer 3b capture mode event interrupt mask
    TIMER3_IMR_R |= 0x0400;

    // Enable the timer 3b interrupt
    NVIC_EN1_R |= 0x10;
    // set the priority value of this interrupt to 0b001
    NVIC_PRI9_R |= 0x20;

    // enable timer 3b
    TIMER3_CTL_R |= 0x0100;



}

void ping_trigger (void){
    STATE = LOW;

    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x0100;
    TIMER3_IMR_R &= ~0x0400;
    NVIC_EN1_R &= ~0x10;

    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_DIR_R |= 0x08;
    GPIO_PORTB_AFSEL_R &= ~0x08;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    GPIO_PORTB_DATA_R &= ~0x08; // set to low
    unsigned int startTime = timer_getMicros();
    GPIO_PORTB_DATA_R |=  0x08; // high
    while( timer_getMicros() - startTime < 5 ) {  /* Wait for 5 uS */ };
    GPIO_PORTB_DATA_R &= ~0x08; // low

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x0400;

    // Re-enable alternate function, timer interrupt, and timer
        // (No need to write to the DIR register for port b, since it'll be controlled by the alternate function.)
    GPIO_PORTB_AFSEL_R |= 0x08;
    TIMER3_IMR_R |= 0x0400;
    TIMER3_CTL_R |= 0x0100;
    NVIC_EN1_R |= 0x10;
}

void TIMER3B_Handler(void){

  // YOUR CODE HERE
  // As needed, go back to review your interrupt handler code for the UART lab.
  // What are the first lines of code in the ISR? Regardless of the device, interrupt handling
  // includes checking the source of the interrupt and clearing the interrupt status bit.
  // Checking the source: test the MIS bit in the MIS register (is the ISR executing
  // because the input capture event happened and interrupts were enabled for that event?
  // Clearing the interrupt: set the ICR bit (so that same event doesn't trigger another interrupt)
  // The rest of the code in the ISR depends on actions needed when the event happens.

    if(TIMER3_MIS_R & 0x0400){

        TIMER3_ICR_R |= 0x400;

        // i++; // This breaks the printf?
        // lcd_printf("Can of whoopAss!\n%d", i);

        if(STATE == LOW){
            START_TIME = TIMER3_TBR_R;
            STATE = HIGH;
        }
        else if(STATE == HIGH){
            END_TIME = TIMER3_TBR_R;
            STATE = DONE;
        }
        else{
            // fuck you pal!
        }



    }

}

float ping_getDistance (void){

    unsigned long timeDiff_Count;
    unsigned long overflow = END_TIME > START_TIME;
    if(overflow){
        NUM_OVERFLOWS++;
    }
    timeDiff_Count = (overflow<<24) - END_TIME + START_TIME;

    float timerPeriod = 1.048576; // In seconds. Derives from: 0xFFFFFF*(1/(16MHz))
    float percentageTotal = (float)timeDiff_Count/(float)0xFFFFFF;
    float timeDiff_Sec = percentageTotal*timerPeriod;

    float speedOfSound_cm = 34300;
    float distance = (timeDiff_Sec/2)*speedOfSound_cm;

    return distance;

    // YOUR CODE HERE

}
