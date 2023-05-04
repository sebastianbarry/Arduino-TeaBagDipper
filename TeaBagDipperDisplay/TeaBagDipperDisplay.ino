
// Tea Bag Dipper DISPLAY Code

#include "SevSeg.h"
SevSeg sevseg;

#define RX_PIN 0

// Stuff to keep track of time

unsigned long currMillis = 0;
long prevMillis = 0;

// RX Stuff

char key;

// Keep track of timer stuff

int minutes = 0;
int seconds = 0;
int timer_chars[3] = {0,0,0};
bool timerStarted = false;
int displaynumber = 0;

void setup(){
  // Needed for timer display
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};
  // Needed for timer display
  bool resistorsOnSegments = true; 
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_CATHODE; 
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  // Debug purposes
  minutes = 0;
  seconds = 0;
  timerStarted = false;

  // Display 000 on the timer by default
  sevseg.setNumber(000, 2);

  Serial.begin(9600);
  // Serial.println("Initialization done");
}

void loop(){
  currMillis = millis();

  
  if(Serial.available() > 0) {
    key = Serial.read();

    if (key != '-1') {
      // Serial.print(" Read key from RX: ");
      Serial.print(key);
      // Serial.print(" Display reads: ");
      // Serial.print(minutes);
      // Serial.print(seconds);
      // Serial.print(displaynumber);

      if (key == '*') {
        // Serial.println("Timer cleared.");
        minutes = 0;
        timer_chars[0] = 0;
        timer_chars[1] = 0;
        timer_chars[2] = 0;
        seconds = 0;
        timerStarted = false;
        displaynumber = 000;
        sevseg.setNumber(000, 2);
      } else if (key == '#') {
        // Serial.println("Timer started.");
        timerStarted = true;
      } else {

        if (timerStarted) {
          // Serial.println("Timer already started. Clear and start again.");
          return;
        }

        if(timer_chars[2] == 0) {
          timer_chars[2] = key - '0';
        } else if (timer_chars[1] == 0) {
          timer_chars[1] = timer_chars[2];
          timer_chars[2] = key - '0';
        } else if (timer_chars[0] == 0) {
          timer_chars[0] = timer_chars[1];
          timer_chars[1] = timer_chars[2];
          timer_chars[2] = key - '0';
        }
        
        seconds = timer_chars[1] * 10 + timer_chars[2];
        minutes = timer_chars[0];
        //Serial.print("Time set to: ");
        //Serial.print(minutes);
        //Serial.print(":");
        //Serial.println(seconds);

        displaynumber = minutes * 100 + seconds;
        sevseg.setNumber(displaynumber, 2);
      }
    }
  }

  // Every 1 second, execute the code to decrease the timer and display on the timer
  if(currMillis >= prevMillis + 1000) {
    prevMillis = currMillis;

    if (timerStarted) {
      if (seconds == 0 && minutes == 0) {
        // Serial.println("Timer finished.");
        timerStarted = false;
        return;
      }

      if (seconds <= 0) {
        minutes--;
        seconds = 59 + seconds;
      } else {
        seconds--;
      }

      //Serial.print("Time remaining: ");
      //Serial.print(minutes);
      //Serial.print(":");
      //Serial.println(seconds);

      displaynumber = minutes * 100 + seconds;
      sevseg.setNumber(displaynumber, 2);
    }
  }
  sevseg.refreshDisplay();
}