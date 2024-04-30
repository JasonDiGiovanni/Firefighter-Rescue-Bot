#include "servo.h"
#include "Timer.h"
#include "lcd.h"
#include "button.h"
/**
 * main.c
 */
int main(void)
{
    timer_init();
    lcd_init();
    servo_init();

    int servoPos = 0;
    int servoDir = 0; //0 = clockwise   1 = counterclockwise

    timer_waitMillis(1000);
    servoPos = 90;
    servo_move(servoPos);

    timer_waitMillis(1000);
    servoPos = 30;
    servo_move(servoPos);

    timer_waitMillis(1000);
    servoPos = 150;
    servo_move(servoPos);

    timer_waitMillis(1000);
    servoPos = 90;
    servo_move(servoPos);

	while(1);
}
