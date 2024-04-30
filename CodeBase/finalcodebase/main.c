///////////////////////////////////////////////////////////////////////mng
// The REAL main.c                                                   //
////////////////////////////////////////////////////////////////////////*

#include <stdlib.h>
#include "Timer.h"
#include "lcd.h"
#include "uart-interrupt.h"
//#include "cyBot_Scan.h"  // For scan sensors
//#include "puttyFunctions.h"
//#include "dualSensorScan.h"
#include "open_interface.h"
#include "adc.h"
#include "scanplot.h"
#include "move.h"


// TODO maybe bring back?
// extern volatile char recieved_char;

#warning "Possible unimplemented functions"
#define REPLACEME 0

/*
 * just prints a string to whatever console that is being used, no frills
 * only use for debug purposes
 */
void printToMonitor(char *words)
{
    int j = 0;
    for (j = 0; j < strlen(words); j++)
    {
        uart_sendChar(words[j]);
    }
}

void RickRoll(){

    unsigned char notes[] = {68, 70, 73, 70, 77, 77, 75, 68, 70, 73, 70, 75, 75, 73, 72, 70, 68, 70, 73, 70, 73, 75, 72, 70, 68, 68, 75, 73};
    unsigned char duration[] = {8, 8, 8, 8, 24, 24, 48, 8, 8, 8,8, 24, 24, 24, 8, 16, 8, 8, 8, 8, 24, 24, 24, 8, 32, 16, 32, 48};

    // Load the song sequence
    oi_loadSong(1, 28, notes, duration);

    oi_play_song(1); // Play
}

void BadToTheBone(){
    unsigned char notes[] = {55, 55, 60, 55, 58, 59, 55};
    unsigned char duration[] = {12, 12, 12, 12, 4, 12, 16};

    // Load the song sequence
    oi_loadSong(0, 7, notes, duration);

    oi_play_song(0); // Play
}



// REGULAR MAIN //

int main_MAIN (void){
    timer_init();
    lcd_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    uart_interrupt_init();
    adc_init();
    ping_init();
    servo_init();

    oi_setWheels(0,0);




    //initializes the list that will temporarily hold the data points from the last scan only
    //if the cybot scans every other angle then only 90 data points can possibly be collected
    plotPointList list;
    plotPointList_init(&list, 90);


    int i = 0;

    //sends a response to python so the program can begin
    uart_sendChar('g');

    /*
    * this while loop will look for any potential interrupt flags triggered by the gui
    * this should contain any possible manual commands we would want to send to the cybot
    */

    while(command_flag != 2){
        //starts a scan that will save a set of points to the list array

        if(command_flag == 1){
            command_flag = 0;
            plotInit();
            printToMonitor("*");

            fullScan(&list);
            //sends data
            for(i = 0; i < list.length; i++){
                sendData_point(list.pointList[i], "obstacle");
            }
            plotPointList_reset(&list);
            printToMonitor("#");
        }
        //tells the cybot to move a specified distance, the movement code will handle plotting any bumps or cliff ideals
        //the movement code is responsible for updating the robots position
        if(command_flag == 3) {
            command_flag = 0;

            float distance = 0;
            printToMonitor("*");

            distance = atof((const char *)dataIn);
            moveDist(distance, 100, sensor_data, &list);
            plotPointList_reset(&list);
            printToMonitor("#");

        }
        //tells the cybot to rotate a specified amount
        //the movement code is responsible for updating the robots rotation and for plotting any bumps
        if(command_flag == 4) {
            command_flag = 0;
            int angle = 0;
            printToMonitor("*");

            angle = atof((const char *)dataIn);
            turnToAngle(angle ,sensor_data, &list);
            plotPointList_reset(&list);
            printToMonitor("#");


            //psuedo code vvvvv
            //rotate(angle);
            //plotPointList_reset(list);
        }

        if(command_flag == 5){
            command_flag = 0;

            // command flag 5 is a placeholder
            //the rest of these if statements will be for extra stuff like activating the music or compressed air servo
        }

        //compressed air servo
        if(command_flag == 6){
            command_flag = 0;
            uart_sendChar('$');
//            for(i = 0; i < 3; i++){
//                timer_waitMillis(1000);
//                uart_sendChar('$');
//            }
        }

        if(command_flag == 7){
            command_flag = 0;
            RickRoll();
            BadToTheBone();
        }
    }
    oi_free(sensor_data);
    plotPointList_free(&list);

    return 0;

}

// SERVO CALIBRATION MAIN //

int main_SERVO(void){

    uart_interrupt_init();
    lcd_init();
    timer_init();
    adc_init();
    servo_init();

    while(1){



        if(command_flag == 1){
            servo_move(0);
            command_flag = 0;
        }
        if(command_flag == 3){
            servo_move(180);
            command_flag = 0;
        }
        if(command_flag == 4) {

            int angle = 0;
            angle = atof((const char *)dataIn);
            servo_move(angle);
            command_flag = 0;
        }
        if(command_flag == 2){
            break;
        }

    }

    return 0;

}


// IR CALIBRATION MAIN //

int main_IR (void){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    uart_interrupt_init();
    lcd_init();
    timer_init();
    adc_init();

    while(1){
        if(command_flag == 1){
            sendData_IR(adc_read());
            command_flag = 0;
        }
    }


}

int main (void){

//    main_IR();
//    main_SERVO();
    main_MAIN();

}
