#include <stdio.h>
#include <stdlib.h>
#include "cyBot_uart.h"
#include "lcd.h"
#include "timer.h"
#include "puttyFunctions.h"
#include "cyBot_Scan.h"

// distAngleArrays definition and methods //


objData* init_objData(int numObjects, anglePair* inputPairs, float* inputDistances){

    objData* od = calloc(1, sizeof(objData));
    od->anglePairs = inputPairs;
    od->distanceArray = inputDistances;
    od->realWidth = calloc(numObjects, sizeof(float));
    od->radialWidth = calloc(numObjects, sizeof(int));
    od->numberObjects = numObjects;
    return od;

}

distAngleArrays* init_distAngArr(int numMeasurements){

    distAngleArrays* daa = calloc(1, sizeof(distAngleArrays));
    daa->angleArray = calloc(numMeasurements, sizeof(int));
    daa->distanceArray = calloc(numMeasurements, sizeof(float));
    daa->numMeasurements = numMeasurements;
    return daa;

}

void free_objData(objData* od){

    free(od->realWidth);
    free(od->radialWidth);
    free(od->anglePairs);
    free(od->distanceArray);

    free(od);
}
void free_distAngleArrays(distAngleArrays* daa){

    free(daa->angleArray);
    free(daa->distanceArray);
    free(daa->angleArray);

    free(daa);
}

// others //

/**
 * Print a string to Puddy.
 */
void sPuddy(char *message)
{
    char letter = message[0];
    int i = 0;

    while (letter != '\0')
    {
        cyBot_sendByte(letter);
        i++;
        letter = message[i];
    }

    cyBot_sendByte('\r');
    cyBot_sendByte('\n');
    cyBot_sendByte('\n');

}


/*
 * Print a scan to Puddy.
 */
void mineSweep(void){

    sPuddy("\n\n\n");
    sPuddy("Distance measurements:");

    right_calibration_value = 253750; //data values from calibration program
    left_calibration_value = 1188250; //data values from calibration program

    int i;
    cyBOT_Scan_t* scan; // WHY DID I WORK w/out CALLOC? (TA TIME)
    calloc(1, sizeof(cyBOT_Scan_t));

    //loop simply goes through full range of servo aka "mine sweeper"
    for(i=0; i <= 180; i+=2){
        cyBOT_Scan(i, scan);
        mineScannerInfo(i,scan->sound_dist);
    }

    free(scan);
}

/*
 * Print an angle and distance to Puddy.
 */
void mineScannerInfo(int angle, float distance){

    char message[500];
    sprintf(message, "Angle: %d       Distance: %f", angle, distance);

    sPuddy(message);
}

/* Purpose: To produce two arrays for distance and angle measurements from data derived by the cyBot's PING))) scanner
 * int deltaTheta: the change in angle for each measurement (1 degree per measurement, 2 degrees per measurement, etc.)
 * int numMeasurements: the number of measurements the cybot takes
 * int startAngle: the angle that the cybot starts scanning at
 * Returns: int array with numMeasurements number of elements
 *
 *
 */
distAngleArrays* getDistData(int startAngle, int numMeasurements, int deltaTheta, int numScans){

    // Create the distance and angle arrays to be returned
    distAngleArrays* daa = init_distAngArr(numMeasurements);

    // calibrate the scanner
    right_calibration_value = 253750; //data values from calibration program
    left_calibration_value = 1188250; //data values from calibration program
    cyBOT_Scan_t* scan = calloc(1, sizeof(cyBOT_Scan_t));

    int i;
    // For each loop, scans for a distance at an angle, then puts that information into the distance and angle arrays.
    for(i=0; i < numMeasurements; i++){
        int currentAngle = startAngle + i*deltaTheta;

        // Averages numScans distance measurements
        float avgDist = 0;
        int j;
        for(j = 0; j < numScans; j++){
            cyBOT_Scan(currentAngle, scan);
            avgDist += scan->sound_dist;
            mineScannerInfo(currentAngle, scan->sound_dist); // DEBUG
        }
        avgDist /= numScans;

        // Stores distance and angle info.
        daa->distanceArray[i] = avgDist;
        daa->angleArray[i] = currentAngle;


    }

    // Free calloc()'d vars
    free(scan);

    return daa;

}

/*
 * Purpose: To identify objects from the information obtained from the getDistData() function
 * Takes as input a struct representing the distances measured by the bot and the angles they were measured at
 * distAngleArrays* daa: the data returned by getDistData()
 * Returns: objData struct representing several objects
 */
objData* getObject(distAngleArrays* daa){

    /*
     * numMeas: the number of measurements taken by the cybot
     * distArr: the float array representing distance measurements produced by getDistData()
     * angArr:the float array representing angle measurements produced by getDistData()
     */
    int numMeas = daa->numMeasurements;
    float* distArr = daa->distanceArray;
    int* angArr = daa->angleArray;

    // A number greater than the maximum number of objects that could be detected by this funtion
    int maxNumObjects = numMeas/2;

    /*
     * !! calloc() allocated variables !!
     * anglePairsTemp:  A array of two-element int arrays that each represents the start and eng angle at which
     *                  an object was detected at.
     *                  DO NOT DEALLOCATE!! This memory is reassigned to the struct returned by this function.
     * distancesTemp:   A array of floats that each represents the distance at which
     *                  an object was detected at.
     *                  DO NOT DEALLOCATE!! This memory is reassigned to the struct returned by this function.
     * boolArray:       An array of 1's and 0's.
     *                  1: The difference between distArray[i] and distArray[i+1] is within the tolerance for potential objects
     *                  0: The difference between distArray[i] and distArray[i+1] is not within the tolerance
     */
    anglePair* anglePairsTemp = calloc(maxNumObjects, sizeof(anglePair));
    float* distancesTemp = calloc(maxNumObjects, sizeof(float));
    int* boolArray = calloc(numMeas-1, sizeof(int));

    // The number of objects detected
    int objectCount = 0;
    // iteration variables (used only for loops)
    int i, j, g;

    // Number of cm that two distance measurements can be within to still be considered an object.
    float tolerance = 2.0;

    float distCutoff = 135.0; // arbirary large value

    for(i = 0; i < numMeas-1; i++){
       int partOfObject = distArr[i] < distCutoff && fabs(distArr[i] - distArr[i+1]) <= tolerance;
       boolArray[i] = partOfObject;
    }


    /**
     * start: The first index of the boolean array that an objects starts at
     * end: The index of the boolean array that an object ends at
     * numOnes: The number of ones detected in a sequence from the boolean array
     * numZeros: The number of zeros detected in a sequence from the boolean array
     *
     */
    int start = 0;
    int end = 0;
    int numOnes = 0;
    int numZeros = 0;
    int foundObj = 0;

    /*
     * Goes through the boolean array. If there are three TRUE's in a row, there is an object, so keep scanning until the object ends.
     * An object is only considered ended once more than one FALSE appears in a row in the boolean array.
     */
    for (j = 0; j < numMeas -1; j++){

        // Check for TRUE: Potentially start the object, increment numOnes, set numZeroes to 0
        if (boolArray[j]){
            // If this is the first TRUE: start the object at this index.
            if(numOnes==0) { start = j; }
            numOnes++;
            numZeros = 0;
        }
        // Check for FALSE: Increment numZeroes
        else{
            numZeros++;
        }

        // Check for the end of an object: Set end, set foundObj to 1
        if (numOnes >= 3 && numZeros >= 1){
            end = j - 1;
            foundObj = 1;
        }
        // Check for an object that exists at the very end of the boolean array: Set end, set foundObj to 1
        else if (j == numMeas-2 && numOnes >= 3 && boolArray[j]==1){
            end = j;
            foundObj = 1;
        }

        // DEBUG
        if(j % 20 == 0){
            int integer = 1;
        }

        // Check for a found object: Set angle and distance data for the new object
        if(foundObj){
            int startAng = angArr[start];
            int endAng = angArr[end];

            anglePairsTemp[objectCount].pair[0] = startAng;
            anglePairsTemp[objectCount].pair[1] = endAng;
            distancesTemp[objectCount] = distArr[(start + end)/2];

            foundObj = 0;
            objectCount++;
        }

        // If not enough ones exist, and more than one zeros appears, set numOnes to 0.
        if(numZeros >= 1){
            numOnes = 0;
        }

    }

    // Create the struct that holds all the object data
    objData* od = init_objData(objectCount, anglePairsTemp, distancesTemp);

    // From the objects found, compute their radial and real width
    for(g = 0; g < od->numberObjects; g++){

        int startAng = anglePairsTemp[g].pair[0];
        int endAng = anglePairsTemp[g].pair[1];
        char message[550];
        sprintf(message, "%d, %d", startAng, endAng);
        sPuddy(message);
        od->radialWidth[g] = fabs(endAng - startAng);
        od->realWidth[g] = calcWidth(od->radialWidth[g], od->distanceArray[g]);
    }

    // Free calloc()'d memory
    free(boolArray);

    return od;
}


float calcWidth(int angularWidth, float distance){
    return 2*3.14159*distance*angularWidth*(1.0/360.0);
}

float angleFromPair(int* anglePair){
    return (anglePair[0] + anglePair[1]) / 2.0;
}

/**
 * Print all the objects in an objData struct.
 */
void printfBombsFound(objData* obj){

    int i;
    float smallestWidth = 180;
    int smallestObj = 0;
    anglePair* objAnglePairs = obj->anglePairs;

    if(obj->numberObjects > 0){
        float tempWidth;
        smallestObj = 0;
        smallestWidth = obj->radialWidth[0];
        for(i = 1; i<obj->numberObjects; i++){
            tempWidth = obj->radialWidth[i];
            if(smallestWidth > tempWidth){
                smallestWidth = tempWidth;
                smallestObj = i;
            }
        }
    }

    for(i = 0; i<obj->numberObjects; i++){
        char message[500];
        float angle = angleFromPair(objAnglePairs[i].pair);
        float distance = obj->distanceArray[i];
        float radialWidth = obj->radialWidth[i];
        sprintf(message, "Object %d: Angle: %f, Distance: %f, Radial Width %f", i+1, angle, distance, radialWidth);

        sPuddy(message);
    }

    right_calibration_value = 253750; //data values from calibration program
    left_calibration_value = 1188250; //data values from calibration program
    cyBOT_Scan_t* scan = calloc(1, sizeof(cyBOT_Scan_t));

    float smallestItemAngle = angleFromPair(objAnglePairs[smallestObj].pair);

    cyBOT_Scan(smallestItemAngle, scan);

    free(scan);


    sPuddy("finished!");

}
















