typedef struct{
    int numMeasurements;
    float* distanceArray;
    int* angleArray;

} distAngleArrays;

typedef struct{

    int pair[2];

}anglePair;

typedef struct{
    int numberObjects;

    anglePair* anglePairs;
    float* distanceArray;

    float* realWidth;
    int* radialWidth;

} objData;

void sPuddy(char* message);
void mineSweep(void);
void mineScannerInfo(int angle, float distance);
objData* init_objData(int numObjects, anglePair* inputPairs, float* inputDistances);
distAngleArrays* init_distAngArr(int numMeasurements);
void free_objData(objData* od);
void free_distAngleArrays(distAngleArrays* daa);
distAngleArrays* getDistData(int startAngle, int numMeasurements, int deltaTheta, int numScans);
objData* getObject(distAngleArrays* daa);
float calcWidth(int angularWidth, float distance);
void printfBombsFound(objData* obj);
void mineSweep_ControlFlags();
