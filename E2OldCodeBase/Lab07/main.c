///////////////////////////////////////////////////////////////////////mng
// The REAL main.c                                                   //
////////////////////////////////////////////////////////////////////////*

#include <stdlib.h>
#include "Timer.h"
#include "lcd.h"
#include "uart-interrupt.h"
#include "cyBot_Scan.h"  // For scan sensors
#include "puttyFunctions.h"
#include "dualSensorScan.h"
#include "open_interface.h"
#include "adc.h"

// TODO maybe bring back?
// extern volatile char recieved_char;

#warning "Possible unimplemented functions"
#define REPLACEME 0

int main_lab8(void){
        oi_t* cyBot_data = oi_alloc();
        oi_init(cyBot_data);
        timer_init(); // Must be called before lcd_init(), which uses timer functions
        lcd_init();
        uart_interrupt_init();
        cyBOT_init_Scan(0b0011);
        adc_init(0x0); //  why must I be called before?
        sPuddy("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");


        sPuddy("I'm running!");


        while(1){
            if (command_flag == 1){

    //            moveDist(-40, 100, cyBot_data);

                command_flag = 0;
                objectList objList = scanForObjects(0, 180, 2, 4);
                if(objList.numObjects < 1){
                    sPuddy("Found no objects!");
                }
                else{
                    printObjects(objList);
                    goToThinnestObj(objList, cyBot_data);
                }
                objectList_free(&objList);

                //turnToRight(90, cyBot_data);

                break;

            }
        }
        oi_free(cyBot_data);


        return 0;

}

int main_lab7(void)
{

    oi_t* cyBot_data = oi_alloc();
    oi_init(cyBot_data);
    timer_init(); // Must be called before lcd_init(), which uses timer functions
    lcd_init();
    uart_interrupt_init();
    cyBOT_init_Scan(0b0111);
    sPuddy("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");



    sPuddy("I'm running!");


    while(1){
        if (command_flag == 1){

//            moveDist(-40, 100, cyBot_data);

            command_flag = 0;
            objectList objList = scanForObjects(0, 180, 2, 3);
            if(objList.numObjects < 1){
                sPuddy("Found no objects!");
            }
            else{
                printObjects(objList);
                goToThinnestObj(objList, cyBot_data);
            }
            objectList_free(&objList);

            //turnToRight(90, cyBot_data);

            break;

        }
    }
    oi_free(cyBot_data);


    return 0;

}

//*/

//	        int volt = scanIR_volts(0);
//	        char message[500] = "";
//	        sprintf(message, "voltage: %d", volt);
//	        sPuddy(message);

//	        float dist = scanIR_dist(0); //gets the distance at a certain angle
//	        char message[500] = "";
//	        sprintf(message, "distance: %f", dist);
//	        sPuddy(message);

//	        objectList objList = scanForObjects(0, 180, 2, 3);
//	        printObjects(objList);
//
//	        objectList_free(&objList);




 ///////////////////////////////////////////////////////////////////////
 // Servo-calibration main. Comment out when running the actual main. //
 ///////////////////////////////////////////////////////////////////////


#include "cyBot_uart.h"

 int main_calibration(void){

 timer_init();
 lcd_init();
 cyBOT_init_Scan(0b111);
 cyBOT_SERVO_cal();

 // got right:   243250
 // got left:    1225000

 return 0;

 }

 int main(void){

     //main_lab7();
     //main_calibration();
     main_lab8();

 }


