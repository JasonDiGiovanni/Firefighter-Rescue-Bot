/*
 * objects.h
 *
 *  Created on: Mar 13, 2024
 *      Author: weyderts
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_


///////////////////////////////////////////////////////////////////////
// Structs                                                           //
///////////////////////////////////////////////////////////////////////

typedef struct {
    int startStopAngles[2];
    float distance;

    int radialWidth;
    float realWidth;

} object;

typedef struct {
    int numObjects;
    object* objArray;
    // TODO function pointers to "class" methods

} objectList;


///////////////////////////////////////////////////////////////////////
// Functions                                                         //
///////////////////////////////////////////////////////////////////////

objectList objectList_init(int numObjects);
void objectList_free();
void objectList_resize(int newSize, objectList* ObjList);
int objectList_setObjAngles(int index, int startAngle, int endAngle, objectList* ObjList);
int objectList_setDist(int index, float distance, objectList* ObjList);
int objectList_setRadial(int index, int radialWidth, objectList* ObjList);
int objectList_setReal(int index, float realWidth, objectList* ObjList);



#endif /* OBJECTS_H_ */
