int potIn;  // potentiometer 

int revPin = 5; // increase rpm 
int fwdPin = 6; // decrease rpm

int leftButton = 4; // push button
int middleButton = 3; 
int rightButton = 2;

void setup()
{
 
  
  pinMode(revPin, OUTPUT);
  pinMode(fwdPin, OUTPUT); //outputs
  
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(middleButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);  // input internal pullup resistors
  
  Serial.begin(9600);

}
void loop()
{
  
  int leftPinState = digitalRead(leftButton);
  int middlePinState = digitalRead(middleButton);
  int rightPinState = digitalRead(rightButton);  // different pintstates for buttons
  
  if (leftPinState == LOW) // left button pushed
  {
  potIn = analogRead(A0); //reads pot value
  	int output = potIn /4; //makes number range 0-255 in serial
  
  	analogWrite(fwdPin, output); //rpm output 
  
  	Serial.println(output);
  	delay(100);
  }
  
  else if (middlePinState == LOW) //middle button pushed
  {
  	digitalWrite(fwdPin, HIGH); // sets fan to fast as F boi
    digitalWrite(revPin, LOW);
   
  }
  else if (rightPinState == LOW) //right button pushed
  {
  	digitalWrite(fwdPin, LOW);
    digitalWrite(revPin, LOW);  //both pins low turns fan off
    
  }
}