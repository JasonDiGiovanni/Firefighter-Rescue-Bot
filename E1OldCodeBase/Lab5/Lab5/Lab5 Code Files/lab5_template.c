/**
 * lab5_template.c
 *
 * Template file for CprE 288 Lab 5
 *
 * @author Zhao Zhang, Chad Nelson, Zachary Glanz
 * @date 08/14/2016
 *
 * @author Phillip Jones, updated 6/4/2019
 * @author Diane Rover, updated 2/25/2021, 2/17/2022
 */

#include "button.h"
#include "timer.h"
#include "lcd.h"

#include "uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors


//#warning "Possible unimplemented functions"
#define REPLACEME 0

char uart_receive(void);

int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
	uart_init();

	       int i;
	       char ch;
	       lcd_clear();

	       while(1){
	           char str[21] = "";
	           i=1;
	           while(i < 21){
	               ch = uart_receive();
	               str[i-1] = ch;
	               if(ch == '^H'){
	                   str[i-2] = '\0';
	               }

	               if( i == 20 || ch == '\x0d'){
	                   lcd_puts(str);
	                   lcd_gotoLine(2);
	                   lcd_printf("%s\n%d",str, i);
	                   uart_sendStr(str);
	                   break;
	               }
	               i++;
	               uart_sendChar(ch);
	           }
	           if(uart_receive() == 'c'){
	              lcd_clear();
	              uart_sendChar('\r');
	              uart_sendChar('\n');
	                          }
	       }
}
