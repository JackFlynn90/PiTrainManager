/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
int Redled = 6;
int Yelled = 5;
int Grnled = 3;

boolean RedState, YelState, GrnState = false;

int RedRate = 1;
int YelRate = 10;
int GrnRate = 60;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(Redled, OUTPUT);     
  pinMode(Yelled, OUTPUT);     
  pinMode(Grnled, OUTPUT);     
  Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {

  static unsigned long RedTimer = millis();
  static unsigned long GrnTimer = millis();
  static unsigned long YelTimer = millis();

  //Timer for red led
  if(millis() - RedTimer > 1000/(RedRate*2))
  {
    RedState = !RedState; //Flip value of RedState
    digitalWrite(Redled, RedState); //Write to pin
    RedTimer = millis(); //reset timer
  }
  
  //Timer for Yellow led
  if(millis() - YelTimer > 1000/(YelRate*2))
  {
    YelState = !YelState; //Flip value of YelState
    digitalWrite(Yelled, YelState); //Write to pin
    YelTimer = millis(); //reset timer
  }
  
  //Timer for Green led
  if(millis() - GrnTimer > 1000/(GrnRate*2)) //need to double the rate as this covers "on" and "off" cycle
  {
    GrnState = !GrnState; //Flip value of RedState
    digitalWrite(Grnled, GrnState); //Write to pin
    GrnTimer = millis(); //reset timer
  }
}
