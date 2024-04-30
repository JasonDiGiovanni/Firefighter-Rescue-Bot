/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"
#include <stdint.h>
#include "driverlib/interrupt.h"



volatile unsigned long START_TIME = 0;
volatile unsigned long END_TIME = 0;
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse
volatile int overflow = 0;
void ping_init (void){

  // YOUR CODE HERE
    SYSCTL_RCGCTIMER_R |= 0X08;
    SYSCTL_RCGCGPIO_R |= 0X02;
    GPIO_PORTB_DEN_R |= 0X08;
        while((GPIO_PORTB_DEN_R & 0X08) == 0x0){};
        while((SYSCTL_RCGCGPIO_R & 0x02) == 0x0){}; // 3 for stabilization


    GPIO_PORTB_AFSEL_R |= 0x08;
    GPIO_PORTB_PCTL_R &= ~0xF000;
    GPIO_PORTB_PCTL_R |= 0x00007000;
    //GPTMCTL_R |= 0X

    TIMER3_CTL_R &= ~0x100;
    TIMER3_CFG_R |= 0x4;
    TIMER3_TBMR_R &= ~0x10;
    TIMER3_TBMR_R |= 0x07;
    TIMER3_CTL_R |= 0xC00;

    TIMER3_TBILR_R |= 0xFFFF;
    TIMER3_TBPR_R |= 0xFF;
    TIMER3_ICR_R |= 0x0400;
    //changed from 2 to 4
    TIMER3_IMR_R |= 0x0400;
    //do i need this nvic stuff?
    //i thought i figured out this nvic stuff but the slides say something else
    //NVIC_PRI9_R = 0x4;
    NVIC_EN1_R = 0x10;


    TIMER3_CTL_R |= 0x0100;
    //the slides state that 52 should be the first parameter
    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    //RE ENABLE TIMER 3

}



void ping_trigger (void){
    STATE = LOW;

    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0X0100;
    TIMER3_IMR_R &= ~0X0400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0X08;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE

    //set output
    GPIO_PORTB_DIR_R |= 0x8;
    GPIO_PORTB_DATA_R &= ~0x8;
    //2 us
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x8;

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0X0400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0X08;
    TIMER3_IMR_R |= 0X0400;
    TIMER3_CTL_R |=0X0100;
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


    //this should be the last broken thing, i have no clue how to detect edges and to set the state of the machine
    if (TIMER3_MIS_R & 0x400)
        {
        TIMER3_ICR_R |= 0x400;

            //clear flags


            if(STATE == LOW){
                //catch rising edge
                START_TIME = TIMER3_TBR_R;
                STATE = HIGH;


            } else if (STATE == HIGH){
                //catch falling edge
                END_TIME = TIMER3_TBR_R;
                STATE = DONE;
            }



        }
}

float ping_getDistance (void){

    // YOUR CODE HERE
    ping_trigger();
    while(STATE != DONE);
    if(END_TIME - START_TIME > 0){

    overflow++;


    }
    //return(((START_TIME - END_TIME + (unsigned)overflow<< 24) * .0000000625 * 343)/42);
    return(((START_TIME - END_TIME) * .0000000625 * 343)/2 * 100);
}
