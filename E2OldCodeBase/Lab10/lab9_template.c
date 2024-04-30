/**
 * @file lab9_template.c
 * @author
 * Template file for CprE 288 Lab 9
 */

#include "Timer.h"
#include "lcd.h"
#include "ping.h"


// Uncomment or add any include directives that are needed

#warning "Possible unimplemented functions"
#define REPLACEME 0

int main(void) {
    timer_init(); // Must be called before lcd_init(), which uses timer functions
	lcd_init();
    ping_init();


	// YOUR CODE HERE
	lcd_printf(""); // TODO get an explanation: For some reason, this must be called before use in the handled!


	while(1)
	{
	    timer_waitMillis(1000);
	    ping_trigger();
	    while(STATE != DONE) {}
	    STATE = LOW;
	    // YOUR CODE HERE
	    float dist = ping_getDistance();

	    char message[100];
	    sprintf(message, "You should debug\n");
	    sprintf(message, "%sNOW!\n", message);
	    sprintf(message, "%sDistance: %f\n", message, dist);
	    sprintf(message, "%sOverflows: %d", message, NUM_OVERFLOWS);
        lcd_printf(message);


	}

}
