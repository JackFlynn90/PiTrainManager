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

  //Brigthen red led
  for (int i = 0; i < 255; i++)
  {
    analogWrite(Redled, i);
    delay(10);//reset timer
  }

  delay(500);
  
  //Dim red led
    for (int i = 0; i < 255; i++)
  {
    analogWrite(Redled, 255-i);
    delay(10);//reset timer
  }
 
  //Brigthen all leds
  for (int i = 0; i < 255; i++)
  {
    analogWrite(Redled, i);
    analogWrite(Yelled, i);
    analogWrite(Grnled, i);
    delay(10);//reset timer
  }

  delay(500);
  
  //Dim all leds
    for (int i = 0; i < 255; i++)
  {
    analogWrite(Redled, 255-i);
    analogWrite(Yelled, 255-i);
    analogWrite(Grnled, 255-i);
    delay(10);//reset timer
  }
}
