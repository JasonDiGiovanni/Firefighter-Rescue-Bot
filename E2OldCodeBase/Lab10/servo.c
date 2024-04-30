/*
 * servo.c
 *
 *  Created on: Apr 5, 2024
 *      Author: weyderts
 */

#include <REF_tm4c123gh6pm.h>
#include <stdint.h>


void servo_init(){
    int counterPeriod = 0x4E200; // 20ms for a 16MHz clock

    // Configure port B for PB5
    SYSCTL_RCGCGPIO_R |= 0x02;
    while ( (SYSCTL_PRGPIO_R & 0x02) == 0) {}

    // Set PB5 as a digital output
    GPIO_PORTB_DEN_R |= 0x20;
    // Set PB3 as an alternate function
    GPIO_PORTB_AFSEL_R |= 0x20;
    // Set PB3's alternate function
    GPIO_PORTB_PCTL_R &= ~0x00F00000;
    GPIO_PORTB_PCTL_R |= 0x00700000;

    // Configure and enable the timer
    SYSCTL_RCGCTIMER_R |= 0x02;
    while ( (SYSCTL_PRTIMER_R & 0x02) == 0) {}

    // disable timer 1b
    TIMER1_CTL_R &= ~0x0100;

    // Select the 16-bit timer
    TIMER1_CFG_R &= ~0x7;
    TIMER1_CFG_R |= 0x4;
    // Set timer 1b to count down mode
    TIMER1_TBMR_R &= ~0x10;
    // Set timer 1b to PWM mode, enable PWM
    TIMER1_TBMR_R |= 0xA;
    // Set the max/initial timer and prescaler values
    TIMER1_TBILR_R = counterPeriod & 0xFFFF;
    TIMER1_TBPR_R = counterPeriod >> 16;


    // Timer turns back on in servo_move

}

void servo_move(uint16_t degrees){

    // disable timer 1b
    TIMER1_CTL_R &= ~0x0100;

    float pulseMax = 2;
    float pulseMin = 0.5;
    float timePeriod = 20; // 20ms

    int counterPeriod = 0x4E200; // 20ms for a 16MHz clock
    float pulseInput = (degrees/180.0)*(pulseMax - pulseMin) + pulseMin;
    int counterMatch = (1 - pulseInput/timePeriod)*counterPeriod;

    // Set the match timer and match prescaler values
    TIMER1_TBMATCHR_R = counterMatch & 0xFFFF;
    TIMER1_TBPMR_R = counterMatch >> 16;

    // enable timer 1b
    TIMER1_CTL_R |= 0x0100;

}
