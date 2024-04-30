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
#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#warning "Possible unimplemented functions"
//#define REPLACEME 0

void printToPutty(char thing);

int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	lcd_clear();
	cyBot_uart_init(); // Don't forget to initialize the cyBot UART before trying to use it

	// YOUR CODE HERE
	
	char thing = 0;

	while(1)
	{
	    if(button_getButton() != 0){
	        thing = (char) (button_getButton()+'0');
	        printToPutty(thing);
	        lcd_printf("%d", button_getButton());
	    }
	}
}

void printToPutty(char thing){
    char message[100] = "Button ";
    message [7] = thing;
    message [11] = '\0';
    int i = 0;
    char letter = message[0];

    while(letter != '\0'){
        cyBot_sendByte(letter);
        i++;
        letter = message[i];
    }
}
