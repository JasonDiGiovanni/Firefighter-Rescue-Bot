#include "open_interface.h"
#include "movement.h"
#include "Timer.h"
#include "lcd.h"
#include "cyBot_Scan.h"
#include "cyBot_uart.h"
#include "Map.h"


void scanPointAt(int angle, cyBOT_Scan_t scan){
    cyBOT_Scan(angle, &scan);
}

void scanSonar(Obj objects[], cyBOT_Scan_t scan, oi_t*sensor_data){
    int dist;
    int i=0;
    int j=0;
    int current = 0;
    int scanning = 0;//0 = false 1 = true
    char temp[80];// holds 24 chars + null

    cyBOT_Scan(i, &scan);

    cyBot_getByte();
    for(i=0;i<=180;i+=2){ // Start Scan
        cyBOT_Scan(i, &scan);
        dist = scan.sound_dist;

        sprintf(temp,"Angle:%i\tDistance:%i", i,dist);
        print(temp);



            if(!scanning){ // not currently scanning
                if(dist <= 55){
                objects[current].angleStart = i;// sets the start angle to this angle
                objects[current].distance = dist;
                sprintf(temp,"\nScanObjectStart at angle:%i", i);
                print(temp);
                scanning = 1; //start scan
                }
            }else if(scanning && dist > 55) {
             // we scanning
                objects[current].angleEnd = (i-2); // sets end angle
                //we use i-1 because this is the first angle outside of range.
                sprintf(temp,"\nScanObjectEnd at angle:%i", i);
                print(temp);
                current++;
                scanning = 0; //end scan
            }
        }


    if(scanning){ //edge Case
        objects[current].angleEnd = 180; // sets end angle
        //we use i-1 because this is the first angle outside of range.
        sprintf(temp,"\nScanObjectEnd at angle:%i", 180);
        print(temp);
        current++;
        scanning = 0; //end scan
    }

    int actual = current;
    for(i=0;i<=current;i++){
        //choose obj
      if(objects[i].distance < 55 && objects[i].angleEnd-objects[i].angleStart > 6){
        sprintf(temp, "Object: %i\tangle Start:%i\tanlge End:%i\tDistance:%i",i,objects[i].angleStart,objects[i].angleEnd, objects[i].distance);
        print(temp);
        if(objects[i].angleStart == (objects[i].angleEnd) || objects[i].angleStart == 0 || objects[i].angleEnd == 180){ // should we delete
            print("Removing value");
            for(j=i;j<=current;j++){ // remove from list
                objects[j] = objects[j+1];
            }
            i--;
            actual--;
        }
      }
    }
    // [0] [1] [2] [3] [4]
    // [1] [2] [3] [4] [ran]

    int smallest = 100000;
    int objectLocation;
    for(i=0;i<actual;i++){
        if(objects[i].angleEnd-objects[i].angleStart <= smallest && objects[i].angleEnd-objects[i].angleStart > 6 && objects[i].angleStart < 180 && objects[i].angleStart > 0 && objects[i].angleEnd < 180 && objects[i].angleEnd > 0){
            smallest = objects[i].angleEnd-objects[i].angleStart;
            objectLocation = i;
        }
    }

    int angle = (objects[objectLocation].angleStart+objects[objectLocation].angleEnd)/2;
    sprintf(temp,"Angle:%i",angle);
    print(temp);

    scanPointAt(angle, scan);
    print("Ready to move\n\n");
    cyBot_getByte();
    if(angle > 90){
        turn_right(sensor_data, angle - 90);
        sprintf(temp, "Left:%i",angle - 90);
        print(temp);
    }else if(angle < 90){
        turn_left(sensor_data, 90 - angle);
        sprintf(temp, "Right:%i",angle - 90);
        print(temp);
    } else{
        //pass
    }
    move_forward(sensor_data, objects[objectLocation].distance*10-200);

}

void print(char str[]){
    int i = 0;
    while(str[i]){
        cyBot_sendByte(str[i++]);
    }
    cyBot_sendByte('\n');
    cyBot_sendByte('\r');
}
