#include "open_interface.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "cyBot_uart.h"

typedef struct {
    int distance;
    int angleStart;
    int angleEnd;
} Obj;

void scanSonar(Obj objects[], cyBOT_Scan_t scan, oi_t*sensor_data);

void scanPointAt(int angle, cyBOT_Scan_t scan);

void print(char str[]);
