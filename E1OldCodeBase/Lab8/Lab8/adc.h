/*
 * adc.h
 */

#include <stdint.h>
#include <stdlib.h>
#include <REF_tm4c123gh6pm.h>


void adc_init (void);
uint16_t adc_read (void);
