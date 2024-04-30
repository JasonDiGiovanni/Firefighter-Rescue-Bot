/*
 * adc.c
 *
 *  Created on: Mar 22, 2024
 *      Author: weyderts
 */

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>

#define SLOPE 22.5
#define PHASE 0.214
#define OFFSET 0.42

void adc_init(uint8_t hardwareAvg){
    SYSCTL_RCGCADC_R |= 0x01;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while( (SYSCTL_PRGPIO_R & 0x02) != 0x02 ) { /* busy-wait */ } //TODO
    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;
    while( (SYSCTL_PRADC_R & 0x01) != 0x01 ) { /* busy-wait */ }
    ADC0_PC_R &= ~0x0F;
    ADC0_PC_R |= 0x01; // Does this do anything?
    ADC0_SSPRI_R = 0x0123;
    ADC0_ACTSS_R &= ~0x08;
    ADC0_EMUX_R &= ~0xF000;
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10; // Take input from analog input 10 (AIN10)
    ADC0_SSCTL3_R = 0x06;
    ADC0_IM_R &= ~0x08;
    if(hardwareAvg > 0x07){
        hardwareAvg = 0x00;
    }
    ADC0_SAC_R = hardwareAvg;
    ADC0_ACTSS_R |= 0x08;
}

uint16_t adc_read(){
    uint16_t returnVal = 0;

    ADC0_PSSI_R = 0x08;
    while( ADC0_RIS_R & 0x00 == 0 ) { /* busy-wait */ }
    returnVal = ADC0_SSFIFO3_R & 0xFFF; // Grab 12 bits
    ADC0_ISC_R |= 0x8; // Clear RIS by setting ISC

    return returnVal;
}

float ADCToDistance(int ADC){
    float distance;

    float maxVoltage = 3.3;
    float minVoltage = 0.0;
    float voltage = (ADC / 4096.0)*(maxVoltage - minVoltage);
    distance = SLOPE/(voltage - PHASE) - OFFSET;

    return distance;
}

