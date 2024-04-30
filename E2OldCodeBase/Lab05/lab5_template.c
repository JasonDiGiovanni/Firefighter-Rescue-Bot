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


#include "timer.h"
#include "lcd.h"

#include "cyBot_uart.h"  // Functions for communicating between CyBot and Putty (via UART1)
                         // PuTTy: Baud=115200, 8 data bits, No Flow Control, No Parity, COM1

#include "cyBot_Scan.h"  // Scan using CyBot servo and sensors

#include "uart.h"

#include "puttyFunctions.h"
#include "button.h"


#warning "Possible unimplemented functions"
#define REPLACEME 0



int main(void) {
	button_init();
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();

//	lcd_printf("Ran while");
//
//	// initialize the cyBot UART1 before trying to use it
//
//	// (Uncomment ME for UART init part of lab)
//	cyBot_uart_init_clean();  // Clean UART1 initialization, before running your UART1 GPIO init code

//	// Complete this code for configuring the GPIO PORTB part of UART1 initialization (your UART1 GPIO init code)
//	SYSCTL_RCGCUART_R |= 0b10; //WRITE ME    //pg. 344
//	while ((SYSCTL_PRUART_R & 0b000010) == 0) {};  //pg. 410
//
//    //pg. 914 for registers
//    UART1_IBRD_R &= ~0xFFFF;    // Clears 4 bytes
//    UART1_IBRD_R |= 0x08;
//    UART1_FBRD_R &= ~0x3F;       // Clears 1.5 bytes
//    UART1_FBRD_R |= 0x2C;
//
//    UART1_LCRH_R &= ~0xFF;
//    UART1_LCRH_R |= 0x60;
//
//	SYSCTL_RCGCGPIO_R |= 0b10;
//	while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};
//	GPIO_PORTB_DEN_R |= 0x03;
//	GPIO_PORTB_AFSEL_R |= 0x03;
//	GPIO_PORTB_PCTL_R &= ~0x000000FF;     // Force 0's in the desired locations
//	GPIO_PORTB_PCTL_R |= 0x00000011;     // Force 1's in the desired locations
//		 // Or see the notes for a coding alternative to assign a value to the PCTL field

//  (Uncomment ME for UART init part of lab)
//  cyBot_uart_init_last_half();  // Complete the UART device configuration

	uart_init();





	while(1)
	{

	    int DEBUG_1 = 0;
	    int DEBUG_2 = 1;

	    while(DEBUG_1){
	        char character = uart_receive();
	        uart_sendChar(character);
	        if(character == '\r'){ uart_sendChar('\n'); }
	    }

	    while(DEBUG_2){
	        int numChars = 0;
	        char letters[23] = "";
	        // uart_sendStr(string); //DEBUG

	        while(numChars < 20){
	            char curChar = uart_receive();
	            if(curChar == '\r') {break;}
	            letters[numChars] = curChar;
	            numChars++;

	            } letters[numChars] = '\0';

	            sprintf(letters, "%s%d", letters, numChars);

	            lcd_printf(letters);
	    }



	}


}
