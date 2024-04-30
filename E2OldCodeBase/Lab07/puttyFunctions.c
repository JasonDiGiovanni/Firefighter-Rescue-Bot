#include <stdio.h>
#include <stdlib.h>
#include "cyBot_uart.h"
#include "lcd.h"
#include "timer.h"
#include "puttyFunctions.h"
#include "cyBot_Scan.h"
#include "uart-interrupt.h"


/**
 * Print a string to Puddy.
 */
void sPuddy(char *message)
{
    char letter = message[0];
    int i = 0;

    while (letter != '\0')
    {
        cyBot_sendByte(letter);
        i++;
        letter = message[i];
    }

    cyBot_sendByte('\r');
    cyBot_sendByte('\n');
    cyBot_sendByte('\n');

}


// Reference for using interupts in code.
//void mineSweep_ControlFlags(){
//
//    //uart_receive_nonblocking();
//
//
//    sPuddy("\n\n\n");
//    sPuddy("Distance measurements:");
//
//    right_calibration_value = 243250; //data values from calibration program
//    left_calibration_value = 1225000; //data values from calibration program
//
//    int i;
//    cyBOT_Scan_t* scan; // WHY DID I WORK w/out CALLOC? (TA TIME)
//    calloc(1, sizeof(cyBOT_Scan_t));
//
//    //loop simply goes through full range of servo aka "mine sweeper"
//    for(i=0; i <= 180; i+=2){
//        if (command_flag == 2){
//            break;
//        }
//        cyBOT_Scan(i, scan);
//        mineScannerInfo(i,scan->sound_dist);
//    }
//
//    free(scan);
//
//
//}

//TODO
//cyBOT_Scan_t* createCyScanner()














