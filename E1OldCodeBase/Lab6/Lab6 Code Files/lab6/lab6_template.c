/**
 * lab6_template.c
 *
 * Template file for CprE 288 Lab 6
 *
 * @author Diane Rover, 2/15/2020
 *
 *

#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"  // For scan sensors
#include "uart.h"

// Uncomment or add any include directives that are needed
 #include "open_interface.h"
 #include "movement.h"
 #include "button.h"


#define REPLACEME 0


int main(void) {
	timer_init(); // Must be called before lcd_init(), which uses timer functions
	uart_init();
	cyBOT_Scan_t scan;
	cyBOT_init_Scan(0b0111);

	// YOUR CODE HERE
   int i=0;

	while(1)
    	{
          char received_char = uart_receive_nonblocking();

          if(received_char != 0) {
              if(received_char == 'g') {
                  // initiate sensor scan
                  cyBOT_Scan(i, &scan);
                  i += 2;
              } else if (received_char == 's') {
                  // stop the scan
              }else if (received_char == 'r') {
                  // reset the scan
                  i = 0;
              }
          }


}
}
*/
