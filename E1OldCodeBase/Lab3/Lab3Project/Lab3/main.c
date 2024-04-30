#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Map.h"




void main(){
    oi_t*sensor_data = oi_alloc();
    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
    lcd_init();   // Initialize
    oi_init(sensor_data);
    cyBot_uart_init();
    cyBOT_Scan_t scan;
    cyBOT_init_Scan(0b0111);

    char temp[80];// holds
    sprintf(temp,"\nReady to Scan");
                    print(temp);

    while(1){
        Obj objectList[10];
        scanSonar(objectList, scan, sensor_data);
        oi_free(sensor_data); // free data!!!
    }
    return 0;


}
    /*

    char fromPutty;

    while(true) {

        fromPutty = cyBot_getByte();
                uart_recieve(fromPutty);

            if(fromPutty == 'w') {
                move_forward(sensor_data, 250);
            } /*else if (fromPutty == 'a') {
                turn(sensor_data, 45);
            } else if (fromPutty == 's') {
                move_backwards(sensor_data, 250);
            } else if (fromPutty == 'd') {
                turn(sensor_data, -45);
            } else if (fromPutty == 'p'){
                break;
            } else if (fromPutty == 'm') {
                lcd_puts("Got an m");
                Obj objectList[10];
                scanSonar(objectList, scan, sensor_data);
            } else if (fromPutty == 'q'){
                oi_free(sensor_data);
                return;
            }
        }


void uart_recieve(char c) {
    int i = 0;
    int j = 0;
    char text[] = "Received a ";
    char text2[] = "\r\n";
    for (i; i < 11; i++) {
        cyBot_sendByte(text[i]);
    }
    cyBot_sendByte(c);
    for (j; j < 4; j++) {
        cyBot_sendByte(text2[j]);
    }
}
*/
