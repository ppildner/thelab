#include <Stepper.h>

#define STEPS 32


Stepper myStepper(STEPS, 8, 10, 9, 11);

int pVal = 0; 
int potVal = 0;    

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(2500);
 
}

void loop() {
   pVal = map(analogRead(A0),0,1023,0,2048);
  if(potVal>pVal) {
    myStepper.step(5);
  }
  if(potVal<pVal) {
    myStepper.step(-5);
   }
   pVal = potVal;
   Serial.println(pVal);
  
}
