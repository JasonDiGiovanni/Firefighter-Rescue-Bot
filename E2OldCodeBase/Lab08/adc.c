/*
 * adc.c
 *
 *  Created on: Mar 22, 2024
 *      Author: weyderts
 */

#include "REF_tm4c123gh6pm.h"
#include <stdint.h>
#include <math.h>

#define SLOPE 22.5
#define PHASE 0.214
#define OFFSET 0.42

typedef struct{
    int lookup_ADC[7]; // sorted in ascending order
    float lookup_dist[7];

} lookupTable;

void lookupInit(lookupTable* lt){
    lt->lookup_ADC[0] = 770;
    lt->lookup_ADC[1] = 820;
    lt->lookup_ADC[2] = 870;
    lt->lookup_ADC[3] = 950;
    lt->lookup_ADC[4] = 1070;
    lt->lookup_ADC[5] = 1250;
    lt->lookup_ADC[6] = 1700;

    lt->lookup_dist[0] = 80;
    lt->lookup_dist[1] = 70;
    lt->lookup_dist[2] = 60;
    lt->lookup_dist[3] = 50;
    lt->lookup_dist[4] = 40;
    lt->lookup_dist[5] = 30;
    lt->lookup_dist[6] = 20;

}

float lookupDist(int ADC, lookupTable lt){
    float distance = -1;
    if(ADC <= lt.lookup_ADC[0]){
        distance = lt.lookup_dist[0];
    }
    else if(ADC >= lt.lookup_ADC[6]){
        distance = lt.lookup_dist[6];
    }
    else{
        int i;
        for(i = 0; i < 7-1; i++){
            if(ADC > lt.lookup_ADC[i]){
                float adcPercentage = (ADC - lt.lookup_ADC[i]) / (float)(lt.lookup_ADC[i+1] - lt.lookup_ADC[i]);
                distance = lt.lookup_dist[i] + adcPercentage*(lt.lookup_ADC[i+1] - lt.lookup_ADC[i]);
            }
        }
    }

    return distance;
}

void adc_init(uint8_t hardwareAvg){
    SYSCTL_RCGCADC_R |= 0x01;
    SYSCTL_RCGCGPIO_R |= 0x02;
    while( (SYSCTL_PRGPIO_R & 0x02) != 0x02 ) { /* busy-wait */ } //TODO
    GPIO_PORTB_DIR_R &= ~0x10;
    GPIO_PORTB_AFSEL_R |= 0x10;
    GPIO_PORTB_DEN_R &= ~0x10;
    GPIO_PORTB_AMSEL_R |= 0x10;
    while( (SYSCTL_PRADC_R & 0x01) != 0x01 ) { /* busy-wait */ }
    ADC0_SSPRI_R = 0x0123;
    ADC0_ACTSS_R &= ~0x08;
    ADC0_EMUX_R &= ~0xF000;
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10; // Take input from analog input 10 (AIN10)
    ADC0_SSCTL3_R &= ~0x0F;
    ADC0_SSCTL3_R |= 0x06;
    ADC0_IM_R &= ~0x08;
    // TODO fuck with hardware averaging when you understand why it decreases the ADC output
//    ADC0_CTL_R |= 0x40; // Dither enabled for SAC (Tiva Datasheet pg. 806)
//    if(hardwareAvg > 0x07){
//        hardwareAvg = 0x00;
//    }
//    ADC0_SAC_R = hardwareAvg;
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

////    float maxVoltage = 3.3;
////    float minVoltage = 0.0;
////    float voltage = (ADC / 4096.0)*(maxVoltage - minVoltage);
//    distance = SLOPE/(voltage - PHASE) - OFFSET;

//    distance = -0.00000015911310*ADC*ADC*ADC + 0.00067695791592*ADC*ADC - 0.97380100407054*ADC + 500.74507547437679;

//    lookupTable lt;
//    lookupInit(&lt);
//    distance = lookupDist(ADC, lt);

    distance = 10962743.05272972583771*pow(ADC, -1.78189484700798);

    return distance;
}


uint16_t adc_read_softAvg(int ADC, int numAvg){

    long returnVal = 0;
    int i;
    for(i = 0; i < numAvg; i++){
        returnVal += adc_read();
    }
    returnVal /= numAvg;
    return (uint16_t)returnVal;
}
