#include <LiquidCrystal.h>

// Battery Charge / Discharge testing code - created 04/03/2023 by Brian Gardner, 
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int ps_relay = 2; //This is the pin that drives the relay for the charging power supply
int load_relay = 3; //This is the pin that drives the load for discharging the battery
int batteryPin = A0; //This identifies the pin used for reading battery voltage 
int state = 0; //This sets the state of the machine (three) 0 = charging the battery, 1 = wait and check voltage after charging, 2 = discharge testing, 3 = test complete
int batteryValue; //This is where we temporarily store the value from the A0 line that reads the battery voltage
float voltage;  //This is where we will temporarily store the battery voltage during each iteration of the test
float maths1;  //This is a floating point variable used for temprary math functions
float maths2;  //This is a floating point variable used for temprary math functions
float loadOhms = 2.6; //This is the resistance value of your load in ohms
float watts = 0; //This is where we will track how many watts have been dissapated by the load
int seconds = 0; //This is where we will track how long the test has been running

void setup() {
  // put your setup code here, to run once:


lcd.begin(16, 2); //set up LCD's number of columns and rows
pinMode(ps_relay, OUTPUT); //Tells the IDE that the ps_relay pin is an output
pinMode(load_relay, OUTPUT); //Tells the IDE that the load_relay pin is an output
digitalWrite(ps_relay, HIGH); //if state = 0 then we're chraging the battery from the PS
digitalWrite(load_relay, HIGH); //if state = 0 then we're chraging the battery from the PS
}

void loop() {
  // put your main code here, to run repeatedly:

  lcd.setCursor(0, 1);
  lcd.print("Current State = ");
  lcd.println(state);
  delay(3000);
  lcd.clear();

  if (state == 0)  {

    lcd.print("Charging - State = ");
    lcd.println(state);  //Lets see what state we're in
    lcd.println("Connecting the charger...");
    delay(3000);

    digitalWrite(ps_relay, LOW); //if state = 0 then we're chraging the battery from the PS
    batteryValue = analogRead(batteryPin);  //Read the battery voltage
    voltage = batteryValue * 0.0049;  //calculate battery voltage

    lcd.clear();
    lcd.print("Charging Voltage = ");

    voltage = voltage * 10; //We gotta do this because of our voltage divider network

    lcd.println(voltage);  //Tell the world about that voltage

    if (batteryValue >= 294) {  //If the battery voltage is greater than or equal to 14.4V then

      lcd.println("Disconnecting the charger");
      delay(3000);
      digitalWrite(load_relay, HIGH);  //Just making sure the load is disconnected
      digitalWrite(ps_relay, HIGH); //Disconnect the battery and then
      state = 1;  //Change the state to 'resting' and exit the loop
    }
  }

  if (state == 1) {  //we're letting the battery rest for 30 seconds before testing it for early failure and / or moving on

    lcd.clear();
    lcd.print("Resting the battery - State = ");
    lcd.println(state);  //Lets see what state we're in

    delay (30000);
    batteryValue = analogRead(batteryPin);  //Read the battery voltage

    lcd.clear();
    lcd.print("Resting Voltage = ");
    voltage = batteryValue * 0.0049;
    voltage = voltage * 10; //We gotta do this because of our voltage divider network
    lcd.println(voltage);  //Tell the world about that voltage
    delay(3000);

    if (voltage <= 12.5) {  //If the battery voltage is less than or equal to 12.5V then 

        lcd.clear();
        lcd.println("Early Failure");  //Tell the world it failed and then

        while(true);  //This gets the arduino stuck in a infinie loop, essentially halting it

    } else {
      state = 2;  //The battery is full enough to run the discharge loop, set the state variable to 2, discharge testing

      lcd.println(state);  //Lets see what state we're in
    }
  }

  if (state == 2) {
    digitalWrite(ps_relay, HIGH);  //Make sure the charger is disconnected
    digitalWrite(load_relay, LOW);  //Turn on the load

    lcd.println("Load connected - Battery Discharging");
    delay(3000);
    
    batteryValue = analogRead(batteryPin);  //Read the battery voltage
    voltage = batteryValue * 0.0049; //Convert the battery value to a voltage
    voltage = voltage * 10; //We gotta do this because of our voltage divider network

    lcd.clear();
    lcd.print("Voltage = ");
    lcd.println(voltage);  //Tell the world about that voltage
    
    maths1 = voltage * voltage;  //Part of calculating watts is squaring the voltage
    maths2 = maths1 / loadOhms; //calulate how many watts we've dissapated if nothing changed for one hour
    maths1 = maths2 / 3600; //But, we're checking power dissipation every second, so divide that by 60
    watts = watts + maths1; //We add the power dissipated in the last one second to whatever we had before, accumulating tracked dissipated power
    delay (1000); //We're testing the the battery every second
    seconds++; //Increment seconds by 1

    if (voltage <= 12.5) {  //If the battery voltage is less than or equal to 12.5V then 
        
      state = 3;  //Test complete
      digitalWrite(ps_relay, HIGH);  //Make sure the charger is disconnected
      digitalWrite(load_relay, HIGH);  //Turn on the load
    
      lcd.println(state);  //Lets see what state we're in    Serial.println("TEST COMLPETE");
      delay(3000);
      watts = watts + watts;  // 12.5 volts is about %50 discharged so we'll double the watts before telling the user how many watts the battery capacity is

      lcd.clear();
      lcd.print("Watts = ");
      lcd.println(watts);  //Display how many watts of capacity the battery has
      delay(3000);
      lcd.clear();
      lcd.print("Seconds = ");
      lcd.println(seconds);  //Display how many seconds it took to get there
      
    
      while(true); //Stop the arduino
    } 
    //loop around and do it again till the voltage drops to 12.5
    }
}
