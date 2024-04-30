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
