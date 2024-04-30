/// Simple 'Hello, world' program
/**
 * This program prints "Hello, world" to the LCD screen
 * @author Chad Nelson
 * @date 06/26/2012
 *
 * updated: phjones 9/3/2019
 * Description: Added timer_init call, and including Timer.h
 */

#include "Timer.h"
#include "lcd.h"


int main (void) {

	timer_init(); // Initialize Timer, needed before any LCD screen functions can be called 
	              // and enables time functions (e.g. timer_waitMillis)

	lcd_init();   // Initialize the LCD screen.  This also clears the screen. 


	char messageString[54] = "                    Microcontrollers are lots of fun!";
	char outputString[21];
	outputString[20] = '\0';
	int start = 0;

	// Print "Hello, world" on the LCD
	while (1){

	    timer_waitMillis(300);

	    int i;

	    for(i = 0; i < 20; i++){
	        outputString[i] = messageString[ (i+start)%53 ];
	    }

	    start++;

	    if(messageString[start] == '\0'){
	        start = 0;
	    }

	    lcd_printf(outputString);

	}




	// lcd_puts("Hello, world"); // Replace lcd_printf with lcd_puts
        // step through in debug mode and explain to TA how it works
    
	// NOTE: It is recommended that you use only lcd_init(), lcd_printf(), lcd_putc, and lcd_puts from lcd.h.
       // NOTE: For time functions, see Timer.h

	return 0;
}
