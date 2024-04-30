/*
 * main.c
 *
 *  Created on: Mar 22, 2024
 *      Author: weyderts
 */

#include "adc.h"
#include "Timer.h"
#include "lcd.h"
#include "servo.h"


void main1(){

//    timer_init();
//    lcd_init();
//    adc_init();

    timer_init();
    lcd_init();
    servo_init();

    servo_move(0);

    while(1){

    }


}

void main(){
    main1();
}
