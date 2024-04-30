/*
 * move.c
 *
 *  Created on: Apr 21, 2024
 *      Author: adamhopf
 */
#include "move.h"

#define FALSE 0
#define TRUE 1

#define CORRECT_RIGHT 1
#define CORRECT_LEFT 1

void turnToRight(int angle, oi_t *cyBot_data){
    if(angle<=0){
        return;
    }

    double angleSum = 0;

    while( angleSum < angle ){

        oi_setWheels(-50, 50);
        oi_update(cyBot_data);
        angleSum -= cyBot_data->angle;
        calc_botPos((float)cyBot_data->angle, 0.0);

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
        calc_botPos((float)cyBot_data->angle, 0.0);
    }

    oi_setWheels(0,0);

}

/*
 * this turns to an angle, 90 being north relative to when we started
 * if the current angle is in the negatives it wont always choose the shortest distance to rotate to a specified angle
 * might fix that issue later, probably not tho
 */
void turnToAngle(float angle, oi_t *cyBot_data, plotPointList* list){

//    float angle = (float)ang;
    float currentAngle = get_rotation();

    // sPuddy("I'm turning!"); // DEBUG

    currentAngle = remainderf(currentAngle, 360);
    float angleChange = 0;

    if (currentAngle < angle){
        while(currentAngle < angle){
            oi_setWheels(30, -30);
            oi_update(cyBot_data);
            currentAngle += cyBot_data->angle;
            angleChange += cyBot_data->angle;

            //bump sensor
            if(cyBot_data->bumpRight){
                oi_setWheels(0,0);
                calc_coord(0, 10, list);
                //returnResult.bumped = TRUE;
                sendData_point(list->pointList[0], "wall");
                break;

            } else if (cyBot_data->bumpLeft){
                oi_setWheels(0,0);
                calc_coord(180, 10, list);
                sendData_point(list->pointList[0], "wall");
                //returnResult.bumped = TRUE;
                break;
            }
            //cliff sensor
            if(cyBot_data -> cliffFrontRightSignal > 2600 || cyBot_data -> cliffFrontRightSignal < 500){
                oi_setWheels(0,0);
                calc_coord(0, 10, list);
                //returnResult.bumped = TRUE;
                sendData_point(list->pointList[0], "cliff");
                break;
            }
            else if (cyBot_data -> cliffFrontLeftSignal > 2600 || cyBot_data -> cliffFrontLeftSignal < 500){
                oi_setWheels(0,0);
                calc_coord(180, 10, list);
                sendData_point(list->pointList[0], "cliff");
                //returnResult.bumped = TRUE;
                break;
            }
        }
    }

    else if(currentAngle > angle){
        while(currentAngle > angle){
            oi_setWheels(-30, 30);
            oi_update(cyBot_data);
            currentAngle += cyBot_data->angle;
            angleChange += cyBot_data->angle;
            //bump sensor
            if(cyBot_data->bumpRight){
                oi_setWheels(0,0);
                calc_coord(0, 10, list);
                //returnResult.bumped = TRUE;
                sendData_point(list->pointList[0], "wall");
                break;

            } else if (cyBot_data->bumpLeft){
                oi_setWheels(0,0);
                calc_coord(180, 10, list);
                sendData_point(list->pointList[0], "wall");
                //returnResult.bumped = TRUE;
                break;
            }
            //cliff sensor
            if(cyBot_data -> cliffFrontRightSignal > 2600 || cyBot_data -> cliffFrontRightSignal < 500){
                oi_setWheels(0,0);
                calc_coord(0, 10, list);
                //returnResult.bumped = TRUE;
                sendData_point(list->pointList[0], "cliff");
                break;
            }
            else if (cyBot_data -> cliffFrontLeftSignal > 2600 || cyBot_data -> cliffFrontLeftSignal < 500){
                oi_setWheels(0,0);
                calc_coord(180, 10, list);
                sendData_point(list->pointList[0], "cliff");
                //returnResult.bumped = TRUE;
                break;
            }
        }
    }

    oi_setWheels(0,0);
    calc_botPos(angleChange, 0.0);

}

/*
 * this function moves the cybot a specified distance
 * it breaks if the bumb sensor is triggered and it will soon break if the cliff sensor is triggered
 * it calculates the cybot position by subtracting the intended distance by the distance remaining
 */

moveResult moveDist(float distToMove, int speed, oi_t *cyBot_data, plotPointList* list){
    moveResult returnResult;
    returnResult.bumped = FALSE;
    float distMoved = 0;

    if( distToMove > 0.0 ){
        oi_setWheels(speed*CORRECT_RIGHT, speed*CORRECT_RIGHT);
        while(distToMove > 0.0){
            oi_update(cyBot_data);
            distToMove -= (cyBot_data->distance)/10;
            distMoved += (cyBot_data->distance)/10;
            //bump sensor
            if(cyBot_data->bumpRight){
                oi_setWheels(0,0);
                calc_botPos(0.0, distMoved);

                calc_coord(0, 10, list);
                returnResult.bumped = TRUE;
                sendData_point(list->pointList[0], "wall");
                break;

            } else if (cyBot_data->bumpLeft){
                oi_setWheels(0,0);
                calc_botPos(0.0, distMoved);

                calc_coord(180, 10, list);
                sendData_point(list->pointList[0], "wall");
                returnResult.bumped = TRUE;
                break;
            }
            //cliff sensor
            if(cyBot_data -> cliffFrontRightSignal > 2600 || cyBot_data -> cliffFrontRightSignal < 500){
                oi_setWheels(0,0);
                calc_botPos(0.0, distMoved);

                calc_coord(0, 10, list);
                returnResult.bumped = TRUE;
                sendData_point(list->pointList[0], "cliff");
                break;

            } else if (cyBot_data -> cliffFrontLeftSignal > 2600 || cyBot_data -> cliffFrontLeftSignal < 500){
                oi_setWheels(0,0);
                calc_botPos(0.0, distMoved);

                calc_coord(180, 10, list);
                sendData_point(list->pointList[0], "cliff");
                returnResult.bumped = TRUE;
                break;
            }



        }

        if(returnResult.bumped == FALSE){
        oi_setWheels(0,0);
        calc_botPos(0.0, distMoved);
        returnResult.distRemaining = distToMove;
        }
    }
    else if( distToMove < 0.0){

        oi_setWheels(-speed*CORRECT_RIGHT, -speed*CORRECT_LEFT);
        while(distToMove < 0.0){
            oi_update(cyBot_data);
            distToMove -= (cyBot_data->distance)/10;
            distMoved += (cyBot_data->distance)/10;
        }
        oi_setWheels(0,0);
        calc_botPos(0.0, distMoved);
        returnResult.distRemaining = distToMove;
    }
    else{
        returnResult.bumped = FALSE;
        returnResult.distRemaining = 0.0;

    }


    return returnResult;
}
