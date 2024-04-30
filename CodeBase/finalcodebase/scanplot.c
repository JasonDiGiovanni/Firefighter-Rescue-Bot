/*
 * scanplot.c
 *
 *  Created on: Apr 19, 2024
 *      Author: adamhopf
 */

#include "scanplot.h"

#define END_STRING "_"
#define PI 3.14159265

#define IR_CONST 2870000
#define IR_POW -1.62

float bot_x = 0;
float bot_y = 0;
//start facing
float bot_rotation = 90;
char* botString = "bot";
plotPoint bot = {
   .xPos = 0,
   .yPos = 0
};

//TODO
//make this information accessable by all functions throug main probably i dont fucking know
void cyBot_init(){
    bot.xPos = 0;
    bot.yPos = 0;
    bot_rotation = 0;
}



//TODO
//set size gud, idk how

/**
 * Creates a list of points.
 * plotPointList* ppl - The list to initialize
 * int length - Length of the list
 */
void plotPointList_init(plotPointList* ppl, int maxLength){
//    ppl = calloc(sizeof(plotPointList), 1);
    ppl->maxLength = maxLength;
    ppl->length = 0;
    ppl->pointList = calloc(sizeof(plotPoint), maxLength);

}
//assumes that we have not hit max length number of points
void plotPointList_append(plotPoint point, plotPointList* list){

    if(list->length >= list->maxLength) { return; }
    list->pointList[list->length].xPos = point.xPos;
    list->pointList[list->length].yPos = point.yPos;
    list->length ++;
}

/**
 * Free a plotPointList
 * plotPointList* ppl - The list to free
 */
void plotPointList_free(plotPointList* ppl){
    free(ppl->pointList);
    free(ppl);
}

/**
 * just resets the length variable back to zero so that the allocated data can be reused
 */
void plotPointList_reset(plotPointList* list){
    list->length = 0;
}



/**
 * Updates current position of robot
 * rotation - last rotation movement
 * distance - last distance moved
 * bot_rotation - change in rotation from start of mission
 * bot_x/y - change in position from start of mission
 */
void calc_botPos(float rotation, float distance){
    //this function will calculate the current position of the cybot on the grid and its position will be stored as a global variable
    //every time the cybot moves or turns this function must be called with the rotation and distance to update the cybots relative position
    bot_rotation = bot_rotation + rotation;
    float ang = bot_rotation * (PI/180.0);
    bot_x = (distance * cos(ang)) + bot_x;
    bot_y = (distance * sin(ang)) + bot_y;
    bot.xPos = bot_x;
    bot.yPos = bot_y;
    sendData_point(bot, botString);

}

float get_rotation(){
    return bot_rotation;
}

//not used currently
int findMidAngle(float* anglePair){
    // TODO (not important) Make function find coterminal angles for angles lesser than 0 degrees and greater than 360 degrees.
    return (anglePair[0] + anglePair[1]) / 2.0;
}

/**
 * calculates position of whole objects
 */
void calc_coordObj(object obj, plotPointList* list){
    //once the coordinates are calculated it will be sent as a string separated by a tab character to python to be saved in a file :)
    //calculate aprox angle
    //I know that there is a bunch of mid angle functions but they dont work how i want them to lol

    float ang1 = (float)obj.startStopAngles[0];
    float ang2 = (float)obj.startStopAngles[1];
    float ang = (ang1 + ang2)/2 + bot_rotation;

    //calculate approx distance
    //this code does not take into account the fact that the sensors are not in the center of the cybot
    float dist = obj.distance;
    float yCord = dist * sin(ang) + bot_y;
    float xCord = dist * cos(ang) + bot_x;

    plotPoint plot;
    plot.xPos = xCord;
    plot.yPos = yCord;
    //appends new coordinates to the list using a pointer
    plotPointList_append(plot, list);


}


/**
 * calculates position of single points that are detected by the scan function
 */
void calc_coord(int angle, float dist, plotPointList* list){
    //include total angle after the correction has been made
    float ang = (float)angle;
    ang = ang * (PI/180.0);
    float yCord = dist * sin(ang);
    float xCord = dist * cos(ang);
    ang = atan((yCord + 10)/xCord);
    if(ang < 0){
        ang = ang + PI;
    }
    float newDist = xCord/cos(ang);
    ang = ang + (bot_rotation - 90) * (PI/180.0);
    xCord = newDist * cos(ang) + bot_x;
    yCord = newDist * sin(ang) + bot_y;


    plotPoint plot;
    plot.xPos = xCord;
    plot.yPos = yCord;
    //append the point to da list
    plotPointList_append(plot, list);

}

/**
 * does a full scan from 0 to 180 degrees and sends filtered results to calc_coord
 * irDataInCent - converts dumb ir values into cool and useable values in cm
 * ^^^^^MUST BE CALABRATED^^^^^
 */
int lookingAt = -1;
float lastIRDistance = 0;

void plotInit(){
    lookingAt = -1;
    lastIRDistance = 0;
}

/**
 * Returns IR scan in centimeters
 */
float Scan_IR(){
    return pow(adc_read(), IR_POW) * IR_CONST;
}

/**
 * Scans from 0 degrees to 180 degrees
 */
void fullScan(plotPointList* list){
    float pingDistance = 0;
    float irData = 0;
    float irDataInCent = 0;
    int i = 0;
    for(i = 0; i <= 180; i += 2) {

        //TODO
        //make these modern functions work with simons code
        servo_move(i);
        pingDistance = ping_getDistance();

        // Software averaging!
        int j;
        irDataInCent = 0;
        for(j = 0; j < 4; j++){
            irDataInCent += Scan_IR();
        }
        irDataInCent /= 4;

        //logic to filter out junk data :)
        //idea: make temp plot points that only the ping sensor contributes to inorder to get a general feel for where the cybot must travel
        if((pingDistance > 5 && pingDistance < 60) && (irDataInCent > 10 && irDataInCent < 60)){

            calc_coord(i, irDataInCent, list);


        }
    }
}

/**
 * takes a given point and prints a line through uart to be read by the python script
 * example string:
 * xCord    yCord   objectType
 */
void sendData_point(plotPoint point, char* objectType){
    float xCord = point.xPos;
    float yCord = point.yPos;
    char packet[500] = "";
//    sprintf(packet, "_POINT_"); // append the header
//    sprintf(packet, "%.2f\t%.2f\t%s", xCord, yCord, objectType);
//    sprintf(packet, END_STRING);
    sprintf(packet, "%s%.2f\t%.2f\t%s%s", "_P_", xCord, yCord, objectType, END_STRING);

    int j = 0;
    for (j = 0; j < strlen(packet); j++){
       uart_sendChar(packet[j]);
    }
    timer_waitMillis(100);
}

void sendData_IR(uint16_t ADC){
    char packet[500] = "";
    sprintf(packet, "%s%d%s", "_I_", ADC, END_STRING);

    int j = 0;
    for (j = 0; j < strlen(packet); j++){
        uart_sendChar(packet[j]);
    }
}

