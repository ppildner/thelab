#include <Servo.h>

Servo patrickservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  patrickservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    patrickservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 20ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    patrickservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 20ms for the servo to reach the position
  }
}
