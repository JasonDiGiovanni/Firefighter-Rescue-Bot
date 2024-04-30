/*
 * adc.h
 *
 *  Created on: Mar 22, 2024
 *      Author: weyderts
 */

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>

// Epic struct ;)
typedef struct{
                ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;           ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ;;;;;;;;;;;;;;;;;;;                         ;;;;;;;;;;;;;;;;;;;;;;;;;
} semicolonsHAHA;

void adc_init();
uint16_t adc_read();

float ADCToDistance(int ADC);
uint16_t adc_read_softAvg();

#endif /* ADC_H_ */
