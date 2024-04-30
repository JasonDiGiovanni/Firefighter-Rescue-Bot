/*
 * scanplot.h
 *
 *  Created on: Apr 19, 2024
 *      Author: adamhopf
 */

#ifndef SCANPLOT_H_
#define SCANPLOT_H_

#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "objects.h"
#include <string.h>
#include "uart-interrupt.h"
#include "servo.h"
#include "ping.h"
#include "adc.h"
#include "timer.h"

typedef struct {
    float xPos;
    float yPos;

} plotPoint;

typedef struct{
    int length;
    plotPoint* pointList;
    int maxLength;
} plotPointList;

void plotPointList_init(plotPointList* ppl, int maxLength);
void plotPointList_append(plotPoint point, plotPointList* list);
void plotPointList_free(plotPointList* ppl);
void plotPointList_reset(plotPointList* list);
void sendData_point(plotPoint point, char* objectType);
void calc_botPos(float rotation, float distance);
int findMidAngle(float* anglePair);
void calc_coordObj(object obj, plotPointList* list);
void calc_coord(int angle, float dist, plotPointList* list);
void fullScan(plotPointList* list);
void sendData(plotPoint point, char* objectType);
float get_rotation();
void plotInit();
void cyBot_init();
void sendData_IR(uint16_t ADC);



#endif /* SCANPLOT_H_ */
