/*
 * move.h
 *
 *  Created on: Apr 21, 2024
 *      Author: adamhopf
 */

#ifndef MOVE_H_
#define MOVE_H_

#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "timer.h"
#include "uart-interrupt.h"
#include "objects.h"
#include "open_interface.h"
#include "adc.h"
#include "scanplot.h"

typedef struct{
    int bumped;
    float distRemaining;
} moveResult;

void turnToAngle(float angle, oi_t *cyBot_data, plotPointList* list);
moveResult moveDist(float distToMove, int speed, oi_t *cyBot_data, plotPointList* list);
void turnToRight(int angle, oi_t *cyBot_data);
void turnToLeft(int angle, oi_t *cyBot_data);



#endif /* MOVE_H_ */
