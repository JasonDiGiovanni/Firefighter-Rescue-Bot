/**
 * main.c
 */
/*
#include <inc/tm4c123gh6pm.h>
#include "adc.h"
#include "Timer.h"
#include <uart.h>
#include <cyBot_Scan.h>
#include <lcd.h>
#include <REF_tm4c123gh6pm.h>

int main(void)
{
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    lcd_clear();
    uart_init();
    adc_init();

    int i, j;
        int val = 15;
        uint16_t sum = 0;
        cyBOT_Scan_t scan;
        uint16_t adcValue;
        double adcDistValue;
        char ADC[50];
        char write[50];
        char c;
        while (1) {
            if (uart_receive() == 'f'){

            for (i = 0; i < val; i++) {
                cyBOT_Scan(90, &scan);
                adcValue = adc_read();
                sum += adcValue;
                timer_waitMillis(100);
            }
            uint16_t n = sum / val; //avg
            double inverse = 1.0 / n;
            sum = 0;

                    adcDistValue = 110722*(inverse) - 51.26;
                    sprintf(ADC, "%d", n);
                    uart_sendStr(ADC);
                    uart_sendChar('\r\n');
                    double formatted = adcDistValue;
                    sprintf(write, "%.2f", formatted);
                    uart_sendStr(write);
                    uart_sendChar('\r\n');
        }
        }
}
*/
#include "adc.h"
#include "Timer.h"
#include "lcd.h"
#include "uart.h"

#include <math.h>

int main()
{
    timer_init();
    lcd_init();
    uart_init();
    adc_init();


    uint16_t temp;
    double dist = 0;

    char message[90];

    while(1)
    {//y = 8057.6x^-0.937

        temp = adc_read();
        dist = 8057.6 * pow(temp,-0.937);

        sprintf(message,"IR: %d, Dist: %lf",temp,dist);
        lcd_printf(message);
        sprintf(message,"IR: %d, Dist: %lf \n",temp,dist);
        uart_sendStr(message);
        timer_waitMillis(50);
    }
}
