/**
 * lab4_template.c
 *
 * Template file for CprE 288 lab 4
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 */

#include "button.h"
#include "Timer.h"
#include "lcd.h"
#include "REF_tm4c123gh6pm.h"
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
#include "puttyFunctions.h"     // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
    cyBot_uart_init();



	// Don't forget to initialize the cyBot UART before trying to use it

	// YOUR CODE HERE




	while(1)
	{
	    int buttonStatus = button_getButton();
	    char message[20] = "0";
	    if(buttonStatus){

	        sprintf(message, "%d", buttonStatus);
	        lcd_printf(message);
	        sPuddy(message);
	    }
	    else{
	        lcd_printf("no detection (0)");
	        sPuddy(message);
	    }





	}

}
