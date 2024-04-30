
#include <Arduino.h>
#include <Servo.h>
#include "Talkie.h"
#include "Vocab_US_Large.h"
#define HWSERIAL1 Serial1
Servo sillyServo;
String str;
Talkie voice;



void setup() {
  sillyServo.attach(11); //pin the servo is connected to
  Serial1.begin(115200); //cereal baud
  Serial.begin(9600); //cereal baud of USB serial... yum cereal
}

void loop() {
  


  if (Serial1.available() > 0) {
      str = Serial1.readStringUntil('\r'); //when return carriage is detected end read
      Serial.println(str); //debugging
      if (str == "$") {
        voice.say(sp2_DANGER);
         voice.say(sp2_DANGER);
        voice.say(sp2_FIRE);
        voice.say(sp2_FIRE);
       sillyServo.write(135);
        delay(1000);
        sillyServo.write(90);
         delay(2000);
        voice.say(sp2_COMPLETE);
       
      }
      //if the serial receives an S, then an audio file needs to be played
      if (str == "&") {
        //do something that plays an audio file NOT IMPLEMENTED YET
        voice.say(sp2_DANGER);
        sillyServo.write(135);
        delay(1000);
        sillyServo.write(90);
        delay(2000);
        voice.say(sp2_COMPLETE);
        // sillyServo.write(135);
        // delay(1000);
        // sillyServo.write(90);
      }
    
  }
}