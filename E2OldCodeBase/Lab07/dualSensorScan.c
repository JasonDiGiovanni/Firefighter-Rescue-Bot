///////////////////////////////////////////////////////////////////////
// Includes                                                          //
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include "cyBot_uart.h"
#include "lcd.h"
#include "timer.h"
#include "puttyFunctions.h"
#include "cyBot_Scan.h"
#include "uart-interrupt.h"
#include "objects.h"
#include "open_interface.h"
#include "dualSensorScan.h"
#include "adc.h"

#define FALSE 0
#define TRUE 1
#define PI 3.14159
#define MAX_IR_DIST 60.0
#define MIN_IR_DIST 10.0
#define WIDTH_CYBOT 33.0 // NOT EXACT!!
#define DEGREE_TO_RADIANS 0.017453
#define M4G1K_NUMB3R 10

///////////////////////////////////////////////////////////////////////
// memory-initialization and memory-free functions                   //
///////////////////////////////////////////////////////////////////////

#warning "Memory allocated for returned pointer; must use free() on the returned pointer to prevent memory leak."
/*
 * Allocates memory for a cyBOT_Scan struct.
 * This function also assigns hard-coded calibration values. (Change these as necessary.)
 * TODO Automate finding calibration values?
 */
cyBOT_Scan_t* createCyScanner(){
    right_calibration_value = 306250; //data values from calibration program
    left_calibration_value = 1314250; //data values from calibration program

    cyBOT_Scan_t* scanner = calloc(1, sizeof(cyBOT_Scan_t));
    return scanner;
}

///////////////////////////////////////////////////////////////////////
// IR scans                                                          //
///////////////////////////////////////////////////////////////////////

/*
 * Purpose: Performs a scan with the IR scanner at a specified angle and returns the voltage measured.
 * angle: (int) The angle the IR scan should be taken
 * Returns: (int) a voltage in millivolts.
 */
int scanIR_volts(int angle){
    cyBOT_Scan_t* scanner = createCyScanner();
    cyBOT_Scan(angle, scanner);

    int voltage = scanner->IR_raw_val;

    free(scanner);

    return voltage;
}

/*
 * Purpose: Performs a scan with the IR scanner at a specified angle and returns the distance measured.
 * angle: (int) The angle the IR scan should be taken
 * Returns: (float) a distance in centimeters.
 */
//float scanIR_dist(int angle){
//    cyBOT_Scan_t* scanner = createCyScanner();
//    cyBOT_Scan(angle, scanner);
//
//    int milliVolts = scanner->IR_raw_val;
//    float voltage = milliVolts/1000.0;
//    float distance = 22.5/(voltage - 0.214) - 0.42;
//
//    free(scanner);
//
//    return distance;
//}

float scanIR_dist_adc(int angle){
    cyBOT_Scan_t* scanner = createCyScanner();
    cyBOT_Scan(angle, scanner);

    float distance = ADCToDistance(adc_read());

    free(scanner);

    return distance;
}

/*
 * Purpose: Performs a scan with the PING scanner at a specified angle and returns the distance measured.
 * angle: (int) The angle the PING scan should be taken
 * Returns: (float) a distance in centimeters.
 */
float scanPING_dist(int angle){
    float distance;

    cyBOT_Scan_t* scanner = createCyScanner();
    cyBOT_Scan(angle, scanner);

    distance = scanner->sound_dist;

    free(scanner);

    return distance;
}

/**
 * Purpose: Performs multiple IR scanner scans at a specified angle, then averages the results those scans.
 * angle: (int) The angle the IR scans should be taken.
 * Returns: (float) a distance in centimeters.
 */
float scanIR_dist_Avg(int angle, int numScans){
    int i;
    float sum = 0;
    for(i = 0; i<numScans; i++){
        sum += scanIR_dist_adc(angle);
    }
    return sum/numScans;
}

/**
 * Purpose: Performs multiple PING scanner scans at a specified angle, then averages the results those scans.
 * angle: (int) The angle the PING scans should be taken.
 * Returns: (float) a distance in centimeters.
 */
float scanPING_dist_Avg(int angle, int numScans){
    int i;
    float sum = 0;
    for(i = 0; i<numScans; i++){
        sum += scanPING_dist(angle);
    }
    return sum/numScans;
}

///////////////////////////////////////////////////////////////////////
// Display Output                                                    //
///////////////////////////////////////////////////////////////////////

void mineScannerInfo(int angle, float distance){

    char message[500];
    sprintf(message, "Angle: %d       Distance: %f", angle, distance);

    sPuddy(message);
}

/**
 * Purpose: Sends Putty the results of a 0 to 180 degree scan from the PING sensor.
 */
void mineSweep(void){

    sPuddy("\n\n\n");
    sPuddy("Distance measurements:");

    cyBOT_Scan_t* scan = createCyScanner();

    //loop simply goes through full range of servo aka "mine sweeper"
    int i;
    for(i=0; i <= 180; i+=2){
        cyBOT_Scan(i, scan);
        mineScannerInfo(i,scan->sound_dist);
    }

    free(scan);
}

/**
 * Purpose: Sends data for each object of an objectList to Putty.
 * ObjList: (objectList) The list of objects to send data from to Putty.
 */
void printObjects(objectList ObjList){
    int i;
    char message[500];
    for(i = 0; i < ObjList.numObjects; i++){
        object curObject = ObjList.objArray[i];

        sprintf(message, "Object %d:", i);
        sPuddy(message);

        sprintf(message, "Start Angle, End Angle: %d, %d", curObject.startStopAngles[0], curObject.startStopAngles[1]);
        sPuddy(message);

        sprintf(message, "Distance: %f", curObject.distance);
        sPuddy(message);

        sprintf(message, "Radial Width: %d", curObject.radialWidth);
        sPuddy(message);

        sprintf(message, "Real Width: %f", curObject.realWidth);
        sPuddy(message);

        sPuddy("\n");
    }
}

void printObject(object Obj){
    char message[500];
    sprintf(message, "Start Angle, End Angle: %d, %d", Obj.startStopAngles[0], Obj.startStopAngles[1]);
    sPuddy(message);

    sprintf(message, "Distance: %f", Obj.distance);
    sPuddy(message);

    sprintf(message, "Radial Width: %d", Obj.radialWidth);
    sPuddy(message);

    sprintf(message, "Real Width: %f", Obj.realWidth);
    sPuddy(message);
}

/*
 * Print an angle and distance to Puddy.
 */


///////////////////////////////////////////////////////////////////////
// Non-Scan Functions                                                //
///////////////////////////////////////////////////////////////////////

/**
 * Purpose: Determines if a distance measured is within the valid operating range of the IR scanner.
 * distance_cm: (float) A distance in centimeters.
 * Returns: (int)   0: The distance IS NOT within the operating range of the IR scanner.
 *                  1: The distance IS within the operating range of the IR scanner.
 */
int checkValidRange(float distance_cm){

    /*
     * Hard-coded lower and upper ranges for the IR scanner.
     * These correspond to centimeters.
     */
    float lowerRange, upperRange;
    lowerRange = MIN_IR_DIST;
    upperRange = MAX_IR_DIST;

    // If distance is lower than the lower range for higher than the upper range, then return FALSE (0).
    if( (distance_cm < lowerRange) || (distance_cm > upperRange) ){
        return FALSE;
    }
    // If distance is within an acceptable range, then return TRUE (1).
    else{
        return TRUE;
    }
}

/*
 * Purpose: Finds the integer angle that most-closely bisects two integer angles.
 * anglePair: (int[2]) The angles to be bisected.
 * Returns: (int) The angle that bisects the input angle pair.
 */
int findMidAngle(int* anglePair){
    // TODO (not important) Make function find coterminal angles for angles lesser than 0 degrees and greater than 360 degrees.
    return (anglePair[0] + anglePair[1]) / 2;
}

/*
 * Currently used only for debugging, since it doesn't create any new data.
 * Sends Putty the angle and distance of all the objects in an objectList.
 * Only uses each object's startStopAngles field. (Does not use its distance field-- this function does its own scan for the distances.)
 * objects: (objectList) The objects to send information of to Putty.
 */
void sillyMidAngle(objectList objects){
    int i;
    for (i = 0; i < objects.numObjects; i++){   //goes through the different objects found during the scan

        float distance; //distance that'll be returned later

        int* curAnglePair = objects.objArray[i].startStopAngles;
        int middleAngle = findMidAngle(curAnglePair); //gets the middle angle

        cyBOT_Scan_t* scan = calloc(1, sizeof(cyBOT_Scan_t));

        cyBOT_Scan(middleAngle, scan);
        distance = scan->sound_dist; //returns the distance using the ping sensor

        mineScannerInfo(middleAngle, distance); //prints the object's distance and angle

    }
}



///////////////////////////////////////////////////////////////////////
// Object Detection                                                  //
///////////////////////////////////////////////////////////////////////

/**
 * Purpose: Detects objects using the IR and PING scanners to do a scan sweep between two angles.
 * scanStartAngle: (int) The angle to start a scan sweep from. In degrees.
 * scanEndAngle: (int) The angle to end a scan sweep on. In degrees.
 * deltaTheta: (int) The change in angle from one scan to the next. In degrees.
 * numScansForAvg: (int) The number of scans to average for each angle. Increasing this value drastically increases the execution time of this scan sweep.
 * Returns: (objectList) A list of objects detected in the scan sweep.
 */
objectList scanForObjects(int scanStartAngle, int scanEndAngle, int deltaTheta, int numScansForAvg){
    // Checks for illegal arguments
    // For now, scanStartAngle must be lesser than scanEndAngle.
    if(scanStartAngle >= scanEndAngle){
        return objectList_init(0);
    }
    // For now, deltaTheta must be positive.
    if(deltaTheta <= 0){
        return objectList_init(0);
    }
    // numScanForAvg must be positive.
    if(numScansForAvg <= 0){
        return objectList_init(0);
    }

    // The objectList to return at the end of the function.
    objectList returnList = objectList_init(10);

    /**
     * dist_prev: The distance measured by the previous scan.
     * dist_cur: The distance measured by the current scan.
     * objStartAngle: The angle a newly discovered object is found at.
     * objEndAngle: The angle a newly discovered object ends at.
     * objStartFound: 1 if the start of an object has been found. 0 if the start of an object hasn't been found.
     * objEndFound: 1 if the end of an object has been found. 0 if the end of an object hasn't been found.
     */
    float dist_prev, dist_cur;
    int objStartAngle, objEndAngle;
    int objStartFound, objEndFound;

    // Make an initial scan at scanStartAngle; start the for loop at scanStartAngle+deltaTheta.
    dist_prev = scanIR_dist_Avg(scanStartAngle, numScansForAvg);
    objStartFound = FALSE;
    objEndFound = FALSE;

    // Tolerance (in centimeters) between two measurements to be considered an part of an object.
    float tolerance = 3;
    // Number of objects found by the scan.
    int numObjectsFound = 0;

    // The scan sweep.
    // TODO May have to implement an array of previous scans to guard against one-off bad scans.
    // curAngle: The current angle the scan sweep is scanning at.
    int curAngle;
    for(curAngle = scanStartAngle+deltaTheta; curAngle <= scanEndAngle; curAngle += deltaTheta){
        // Make a new scan to find dist_cur.
        dist_cur = scanIR_dist_Avg(curAngle, numScansForAvg);
        char message[500];
        sprintf(message, "Angle: %d, Distance: %f", curAngle, dist_cur);
        sPuddy(message);

        // TRUE if this is the last loop of the for loop. FALSE otherwise.
        int thisIsLastScan = curAngle + deltaTheta > scanEndAngle;

        /*
         * TRUE if dist_cur is within the accceptable operating region of IR Scanner
         *      AND if the difference between dist_cur adn dist_previous is within the tolerance.
         * FLASE otherwise.
         */
        int acceptableDifference = checkValidRange(dist_cur) && (fabs(dist_cur - dist_prev) < tolerance);

        /*
         * If the start of an object hasn't been found AND there is an acceptable difference between dist_cur and dist_prev:
         * set objStartAngle to the current angle and objStartFound to TRUE.
         */
        if( (objStartFound == FALSE) && acceptableDifference){
            objStartAngle = curAngle;
            objStartFound = TRUE;
        }
        /*
         * If the start of an object has been found AND the end of an object hasn't been found AND there is not an acceptable
         * difference between dist_cur and dist_prev: set objEndAngle to the current angle and objEndFound to TRUE.
         */
        else if( (objStartFound == TRUE) && (objEndFound == FALSE) && !acceptableDifference){
            objEndAngle = curAngle;
            objEndFound = TRUE;
        }
        /*
         * If the start of an object has been found AND the end of an object object hasn't found AND this is the last scan
         * AND there is an acceptable difference between dist_cur and dist_previous:
         * set objEndAngle to the current angle and objEndFound to TRUE.
         * This is an edge case where an object is still being detected on the edge of the scan.
         */
        else if( (objStartFound == TRUE) && (objEndFound == FALSE) && thisIsLastScan && acceptableDifference ){
            objEndAngle = curAngle;
            objEndFound = TRUE;
        }

        /**
         * If the start of an object has been found AND the end of an object has been found:
         * then the scan sweep has detected an object, which needs to be put into the objectList.
         */
        if( (objStartFound == TRUE) && (objEndFound == TRUE) ){
            // If there is not room in the objectList for another object: resize the object list to make room.
            if(numObjectsFound+1 == returnList.numObjects){
                objectList_resize(numObjectsFound+1 + 5, &returnList);
            }

            // Set the start and end angles for the object.
            objectList_setObjAngles(numObjectsFound, objStartAngle, objEndAngle, &returnList);

            // Reset objStartFound and objEndFound so that another object may be detected.
            objStartFound = FALSE;
            objEndFound = FALSE;

            // Increment numObjectsFound, since we have just found an object.
            numObjectsFound++;
        }

        // This scan is now the previous scan.
        dist_prev = dist_cur;
    } //end for

    //

    /*
     * For each object, find their midpoint and use the PING scanner to find the distance the cybot is from that object.
     * Additionally, compute the radial and real widths of the object.
     */
    int falseObjs = 0;
    int i;
    for(i = 0; i < numObjectsFound; i++){
        // Grab the angle of the current potential object
        int* angles = returnList.objArray[i].startStopAngles;
        int midAngle = findMidAngle(angles);

        // Find the distance of the current potential object with the PING scanner
        float distance = scanPING_dist_Avg(midAngle, numScansForAvg);
        sPuddy("Potential object:");
        mineScannerInfo(midAngle, distance);
        // If the distance to the object is farther than the IR sensor's range, it must be a fake object!
        if(distance > MAX_IR_DIST+5){
            sPuddy("Fail");
            falseObjs++;
            continue;
        }
        sPuddy("Success");
        // Calculate radial and real widths for the object
        int radialWidth = abs(angles[1] - angles[0]);
        float realWidth = 2*PI*distance*(radialWidth/360.0);

        // Replace the earliest potential object entry in return list with the data from the current potential object
        objectList_setObjAngles(i-falseObjs, angles[0], angles[1], &returnList);
        objectList_setDist(i-falseObjs, distance, &returnList);
        objectList_setRadial(i-falseObjs, radialWidth, &returnList);
        objectList_setReal(i-falseObjs, realWidth, &returnList);
        printObject(returnList.objArray[i-falseObjs]);
    }

    // Resize the list to fit exactly the number of objects we found during the scan sweep, no more, no less.
    objectList_resize(numObjectsFound-falseObjs, &returnList);
    // Return our newly created objectList.
    return returnList;
}




object getSmallestObj(objectList objList){

    int i;
    object thinnestObj; //used for finding the thinnest object, temp placeholder


    for (i = 0; i < objList.numObjects; i++){
        if (i == 0){
            thinnestObj = objList.objArray[i];
        }

        else if (objList.objArray[i].realWidth < thinnestObj.realWidth){
            thinnestObj = objList.objArray[i];
        }
    }

    return thinnestObj;

}

void turnToAngle(int angle, oi_t *cyBot_data){

    double currentAngle = 90.0;

    // sPuddy("I'm turning!"); // DEBUG

    if (angle < 90){

        while(currentAngle > angle){

            oi_setWheels(-50, 50);
            oi_update(cyBot_data);
            currentAngle += cyBot_data->angle;
//            char message[500];
//            sprintf(message, "%lf", currentAngle);
//            sPuddy(message); // DEBUG
      }
    }

    else {
        while(currentAngle < angle){
            oi_setWheels(50, -50);
            oi_update(cyBot_data);
            currentAngle += cyBot_data->angle;
        }
    }

    oi_setWheels(0,0);

}

void turnToRight(int angle, oi_t *cyBot_data){
    if(angle<=0){
        return;
    }

    double angleSum = 0;

    while( angleSum < angle ){

        oi_setWheels(-50, 50);
        oi_update(cyBot_data);
        angleSum -= cyBot_data->angle;

    }

    oi_setWheels(0,0);

}

void turnToLeft(int angle, oi_t *cyBot_data){
    if(angle<=0){
        return;
    }

    double angleSum = 0;

    while( angleSum < angle ){

        oi_setWheels(50, -50);
        oi_update(cyBot_data);
        angleSum += cyBot_data->angle;

    }

    oi_setWheels(0,0);

}


/**
 * distToMove is in centimeters!!!
 */
moveResult moveDist(float distToMove, int speed, oi_t *cyBot_data){
    moveResult returnResult;
    returnResult.bumped = FALSE;

    if( distToMove > 0.0 ){
        oi_setWheels(speed, speed);
        while(distToMove > 0.0){
            oi_update(cyBot_data);
            distToMove -= (cyBot_data->distance)/10;
            if(cyBot_data->bumpRight || cyBot_data->bumpLeft){
                returnResult.bumped = TRUE;
                break;
            }
        }
        returnResult.distRemaining = distToMove;
    }
    else if( distToMove < 0.0){
        oi_setWheels(-speed, -speed);
        while(distToMove < 0.0){
            oi_update(cyBot_data);
            distToMove -= (cyBot_data->distance)/10;
        }
        returnResult.distRemaining = distToMove;
    }
    else{
        returnResult.bumped = FALSE;
        returnResult.distRemaining = 0.0;
    }
    return returnResult;
}

void goToThinnestObj(objectList objList, oi_t *cyBot_data){

    int SPEED = 200;

    object thinnestObject = getSmallestObj(objList);

    sPuddy("Thinnest Object:");
    printObject(thinnestObject);

    double distanceToObj = (double)thinnestObject.distance;
    int thinObjMidAngle =  findMidAngle(thinnestObject.startStopAngles);

    // move straight for target
    turnToAngle(thinObjMidAngle, cyBot_data);
    moveResult moveData = moveDist(distanceToObj-M4G1K_NUMB3R, SPEED, cyBot_data);
    if(moveData.bumped == 0){
        sPuddy("No Bump!");
        return;
    }

    // got obstructed!
    // TODO create new object for obstructing object.
    float distanceTraveled = distanceToObj - moveData.distRemaining;

    moveDist(-distanceTraveled, SPEED, cyBot_data);

    if(thinObjMidAngle > 90){
        turnToRight(thinObjMidAngle-90, cyBot_data);
    }
    else{
        turnToLeft(90-thinObjMidAngle, cyBot_data);
    }


    sPuddy("Bumped!");

    // determine left-most object and upper-most object.
    object leftMost = objList.objArray[objList.numObjects-1];
    object upperMost = objList.objArray[0];
    int curAngle = findMidAngle(upperMost.startStopAngles);
    float maxVertDist = upperMost.distance*sin(DEGREE_TO_RADIANS*curAngle);
    // object lowerMost = objList.objArray[0]; //TODO?
    int i;
    for(i = 0; i < objList.numObjects; i++){
        //TODO
        object curObject = objList.objArray[i];
        curAngle = findMidAngle(curObject.startStopAngles);
        float curVertDist = curObject.distance*sin(DEGREE_TO_RADIANS*curAngle);
        if(curVertDist > maxVertDist){
            upperMost = curObject;
            maxVertDist = curVertDist;
        }
    }

    sPuddy("Left-most Object:");
    printObject(leftMost);
    sPuddy("Upper-most Object:");
    printObject(upperMost);


    // determine left-dist, up-dist, and right-dist
    float width_leftMost = leftMost.realWidth;
    float dist_leftMost = leftMost.distance;
    int angle_leftMost = findMidAngle(leftMost.startStopAngles);
    float dist_left = width_leftMost + WIDTH_CYBOT + dist_leftMost*fmax(0.0, -cos(DEGREE_TO_RADIANS*angle_leftMost));

    float width_upperMost = upperMost.realWidth;
    float dist_upperMost = upperMost.distance;
    int angle_upperMost = findMidAngle(upperMost.startStopAngles);
    float dist_up = width_upperMost + 2*WIDTH_CYBOT + dist_upperMost*sin(DEGREE_TO_RADIANS*angle_upperMost);

    float width_target = thinnestObject.realWidth;
    float dist_target = thinnestObject.distance;
    int angle_target = findMidAngle(thinnestObject.startStopAngles);
    float dist_right = dist_left + dist_target*cos(DEGREE_TO_RADIANS*angle_target) + (0.5)*width_target;

    char message[500];
    sprintf(message, "Left distance: %f\n Up distance: %f\n Right distance %f\n", dist_left, dist_up, dist_right);
    sPuddy(message);

    // Move left dist
    sPuddy("Moving left");
    turnToAngle(180, cyBot_data);
    moveDist(dist_left, SPEED, cyBot_data);

    // Move up dist
    sPuddy("Moving up");
    turnToAngle(0, cyBot_data);
    moveDist(dist_up, SPEED, cyBot_data);

    // Move right dist
    sPuddy("Moving right");
    turnToAngle(0, cyBot_data);
    moveDist(dist_right, SPEED, cyBot_data);

    // Go straight for the target
    int angleNew_target;
    float distNew_target = dist_up - dist_target*sin(DEGREE_TO_RADIANS*angle_target) - width_target - WIDTH_CYBOT;
    sprintf(message, "Down distance: %f\n", distNew_target);
    sPuddy(message);
    turnToAngle(0, cyBot_data);
    sPuddy("Moving down");
    moveDist(distNew_target-M4G1K_NUMB3R, SPEED, cyBot_data);

    // If obstructed, move back and go delta-Right

    // Repeat previous two steps until success.


    // POTENTIAL IMPROVEMENTS:
    // try to get within SCAN_RANGE of target
    // keep moving around obstructing objects until you can get within range

    oi_setWheels(0,0);
}











