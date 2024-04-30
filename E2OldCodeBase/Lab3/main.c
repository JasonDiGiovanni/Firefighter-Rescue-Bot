#include <stdio.h>
#include "cyBot_uart.h"
#include "lcd.h"
#include "Timer.h"
#include "puttyFunctions.h"
#include "cyBot_Scan.h"

void main(){

    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0111);

    int DEBUG = 0;
    int numLoops = 10;

    while (DEBUG || numLoops){
        if (DEBUG || cyBot_getByte() == 'm'){
            distAngleArrays* data = getDistData(0, 90, 2, 3);
            objData* objects = getObject(data);
            printfBombsFound(objects);

            free_distAngleArrays(data);
            free_objData(objects);

        }
        numLoops--;
    }

}

/* MAIN, CHECKPOINT 2
int main(void){

    timer_init();
    lcd_init();
    cyBot_uart_init();
    cyBOT_init_Scan(0b0111);
    char input;

    while (1){
    if (cyBot_getByte() == 'm'){
        mineSweep();
        }
    }
    return 0;
}
*/

/* MAIN, CHECKPOINT 1
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
    char b = 'C';
    char displayChar;

    timer_init();
    lcd_init();
    cyBot_uart_init();



    lcd_printf("haha");

    cyBot_sendByte(b);

    displayChar = cyBot_getByte();

    lcd_printf("");
    lcd_putc(displayChar);


    char message[50] = "Got a(n) ";

    message[9] = displayChar;
    message[10] = '\0';

    sPuddy(message);
    */
