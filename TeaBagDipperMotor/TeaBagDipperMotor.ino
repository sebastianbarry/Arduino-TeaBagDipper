
// Tea Bag Dipper MOTOR Code

#include <Keypad.h>
#include <Servo.h>

#define TX_PIN = 1;

// Stuff to keep track of time

unsigned long currMillis = 0;
long prevMillis = 0;

// Stuff for Keypad

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {9, 8, 7, 6};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Stuff for motor

int motorPin = 12;
Servo myservo;
int pos = 0;

// Keep track of timer stuff

int minutes = 0;
int seconds = 0;
int timer_chars[3] = {0,0,0};
bool timerStarted = false;

void setup() {
  // Initiate the servo motor
  myservo.attach(motorPin);
  myservo.write(pos);

  Serial.begin(9600);
  // Serial.println("Initialization done");
}

void loop() {
  char key = keypad.getKey();
  currMillis = millis();

  

  if (key != NO_KEY) {
    if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      // ignore non-timer keys
      return;
    }

    // Serial.print(" Sending key on Tx: ")
    Serial.write(key);

    if (key == '*') {
      // Serial.println("Timer cleared.");
      minutes = 0;
      timer_chars[0] = 0;
      timer_chars[1] = 0;
      timer_chars[2] = 0;
      seconds = 0;
      timerStarted = false;
    } else if (key == '#') {
      // Serial.println("Timer started.");
      timerStarted = true;
    } else {
      
      if (timerStarted) {
        //Serial.println("Timer already started. Clear and start again.");
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
    }
  }

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

    //Move the servo every 5 miliseconds by 1 degree out of 180 degrees
    if (currMillis >= prevMillis + 5) {
      prevMillis = currMillis;
      if(pos < 180) {
        for (pos = 0; pos <= 180; pos += 1) { // Move the servo from 0 to 180 degrees
          myservo.write(pos); // Set the servo position
          delay(5); // Delay to allow the servo to move smoothly
        }
      } else if (pos > 0) {
        for (pos = 180; pos >= 0; pos -= 1) { // Move the servo from 180 to 0 degrees
          myservo.write(pos); // Set the servo position
          delay(5); // Delay to allow the servo to move smoothly
        }
      }
    }

    //Serial.print("Time remaining: ");
    //Serial.print(minutes);
    //Serial.print(":");
    //Serial.println(seconds);
  }
  
}