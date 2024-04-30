/*
 * dualSensorScan.h
 *
 *  Created on: Mar 8, 2024
 *      Author: nick5150
 */

#ifndef DUALSENSORSCAN_H_
#define DUALSENSORSCAN_H_

#include "objects.h"
#include "open_interface.h"

typedef struct{
    int bumped;
    float distRemaining;
} moveResult;


float scanIR_dist(int angle);
//cyBOT_Scan_t* createCyScanner();
int scanIR_volts(int angle);
void sillyMidAngle(objectList objects);
objectList scanForObjects(int scanStartAngle, int scanEndAngle, int deltaTheta,
                          int numScansForAvg);
void printObjects(objectList ObjList);
void turnToAngle(int angle, oi_t *cyBot_data);
moveResult moveDist(float distToMove, int speed, oi_t *cyBot_data);
void goToThinnestObj(objectList objList, oi_t *cyBot_data);
#endif /* DUALSENSORSCAN_H_ */
