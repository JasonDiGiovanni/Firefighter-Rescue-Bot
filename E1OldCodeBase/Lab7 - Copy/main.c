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
    while(1){
        Obj objectList[10];
        scanSonar(objectList, scan, sensor_data);
        oi_free(sensor_data); // free data!!!
    }


}