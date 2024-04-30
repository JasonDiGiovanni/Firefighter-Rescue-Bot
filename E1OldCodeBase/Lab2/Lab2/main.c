#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include "lcd.h"


void main(){
    oi_t*sensor_data = oi_alloc();
    timer_init(); // Initialize Timer, needed before any LCD screen functions can be called
    lcd_init();   // Initialize
    oi_init(sensor_data);

    move_forward(sensor_data, 8000);
 /*   timer_waitMillis(1000);


        turn_right(sensor_data, 88);
        timer_waitMillis(1000);


        move_forward(sensor_data, 505);
        timer_waitMillis(1000);


        turn_right(sensor_data, 88);
        timer_waitMillis(1000);


        move_forward(sensor_data, 505);
        timer_waitMillis(1000);


        turn_right(sensor_data, 88);
        timer_waitMillis(1000);


        move_forward(sensor_data, 505);
        timer_waitMillis(1000);
*/

    oi_free(sensor_data);
}