#include "objects.h"
#include <stdlib.h>

#warning "Memory allocated for returned type; must use objectList_free(*objectList) with the returned type to prevent memory leak."
/**
 * Purpose: Initializes an objectList and allocates memory for all fields that need memory.
 * numObjects: (int) The initial size/length of the list. Sets the numObjects field of the struct.
 * Returns: (objectList) The initialized objectList. Uses default values except for the numObjects field.
 */
objectList objectList_init(int numObjects)
{
    objectList returnList;
    returnList.numObjects = numObjects;
    returnList.objArray = calloc(sizeof(object), numObjects);
    return returnList;
}

/**
 * Purpose: Frees all memory relating to an objectList.
 * ObjList: (objectList*) A pointer to the objectList that needs to be freed.
 */
void objectList_free(objectList *ObjList)
{
    free(ObjList->objArray);
    // free(ObjList); // The ObjList is not calloc'd itself. If objectLists are changed to be calloc'd, uncomment this line.
}

/**
 * Purpose: Resizes an objectList. If the new size is less than the old size, the elements from the
 *          old list that can't fit in the new list are thrown out.
 * newSize: (int) the new size of the list.
 */
void objectList_resize(int newSize, objectList *ObjList)
{
    object *newArray = calloc(sizeof(object), newSize);
    object *oldArray = ObjList->objArray;

    int numElementsMoved = ObjList->numObjects;
    if (newSize < numElementsMoved)
    {
        numElementsMoved = newSize;
    }

    int i;
    for (i = 0; i < numElementsMoved; i++)
    {
        newArray[i] = oldArray[i];
    }

    ObjList->objArray = newArray;
    ObjList->numObjects = newSize;
    free(oldArray);

}

/**
 * Purpose: Sets the start and end angles field of an object at a specified index of an objectList.
 * index: (int) The index of the object to set new values for in the list.
 * startAngle: (int) The new value set the object struct field { startStopAngles[0] } to.
 * endAngle: (int) The new value to set the object struct field { startStopAngles[1] } to.
 * Returns: (int)   0: The provided index is outside the bounds of the list.
 *                  1: The provided index is inside the bounds of the list.
 */
int objectList_setObjAngles(int index, int startAngle, int endAngle,
                            objectList *ObjList)
{
    if (index >= ObjList->numObjects)
    {
        return 0;
    }
    int *angles = ObjList->objArray[index].startStopAngles;
    angles[0] = startAngle;
    angles[1] = endAngle;

    return 1;
}

/**
 * Purpose: Sets the distance field of an object at a specified index of an objectList.
 * index: (int) The index of the object to set new values for in the list.
 * distance: (int) The new value set the object struct field { distance } to.
 * Returns: (int)   0: The provided index is outside the bounds of the list.
 *                  1: The provided index is inside the bounds of the list.
 */
int objectList_setDist(int index, float distance, objectList *ObjList)
{
    if (index >= ObjList->numObjects)
    {
        return 0;
    }
    ObjList->objArray[index].distance = distance;

    return 1;
}

/**
 * Purpose: Sets the radial width field of an object at a specified index of an objectList.
 * index: (int) The index of the object to set new values for in the list.
 * radialWidth: (int) The new value set the object struct field { radialWidth } to.
 * Returns: (int)   0: The provided index is outside the bounds of the list.
 *                  1: The provided index is inside the bounds of the list.
 */
int objectList_setRadial(int index, int radialWidth, objectList *ObjList)
{
    if (index >= ObjList->numObjects)
    {
        return 0;
    }
    ObjList->objArray[index].radialWidth = radialWidth;

    return 1;
}

/**
 * Purpose: Sets the real width field of an object at a specified index of an objectList.
 * index: (int) The index of the object to set new values for in the list.
 * distance: (int) The new value set the object struct field { realWidth } to.
 * Returns: (int)   0: The provided index is outside the bounds of the list.
 *                  1: The provided index is inside the bounds of the list.
 */
int objectList_setReal(int index, float realWidth, objectList *ObjList)
{
    if (index >= ObjList->numObjects)
    {
        return 0;
    }
    ObjList->objArray[index].realWidth = realWidth;

    return 1;

}
