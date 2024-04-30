/*
 * main.c
 *
 *  Created on: Mar 22, 2024
 *      Author: weyderts
 */

#include "adc.h"
#include "Timer.h"
#include "lcd.h"



void main1(){

    timer_init();
    lcd_init();
    adc_init();

    int curTime = timer_getMillis();
    int constant = 1000;
    int goalTime = curTime + constant;
    while(1){
        if(curTime > goalTime){
            goalTime += constant;
            int ADCval = adc_read();
            char message[500];
            sprintf(message, "%d", ADCval);
            lcd_printf(message);
        }
        curTime = timer_getMillis();

    }

}

void main(){
    main1();
}
