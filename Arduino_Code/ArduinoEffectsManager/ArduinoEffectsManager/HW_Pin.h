// HW_Pin.h

#ifndef _HW_PIN_h
#define _HW_PIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define LEDON 0
#define LEDOFF 1
/*
HW_Pin Class

Allows me to create functions like "blinking" or "fading" of hardware PWM or digital writes to the assossiated pin. Essentially it's
a helper library on top of the standard arduino functions.
Stores states for easy change of output
Refresh must be called consistently for effects to take place
*/
class HW_PinClass
{
 protected:
	int _PinNum; //Teensy Pin Number
	int _CurrentValue; //Current pin state value 0/1 for digital 0/255 for analogue
	
	int _analogueValue; // Analogue value for PWM
	boolean _PWMEnable = false; //Flag for PWM usage
	
	int _BlinkRate = 0; //Manual blinking using digital on/off
	unsigned long _lastBlink = 0; // Timer for blinking
	boolean _BlinkEnable = false; // Flag for Blinking
	
	boolean _enabled = true; //Pin output hard on/off without changing effects states

	float _FadeValue = 0.0; //Fading current value
	int _FadetargetValue; // Fading target value to achieve
	float _FadeRate = 50.0; // Rate of change for Fading
	boolean _FadeEnable = true; // Flag for fading
	
 public:
	void setup(int PinNumber); //Plain setup for easy on/off toggling of pin
	void setup(int PinNumber, boolean PWMEnable, float FadeRate); //PWM enable setup for allowing PWM usage
 
	void setPin(int Value); //Set pin state digital
	void setAnalogueValue(int analogueSet); //Set pin state PWM
	void setBlinkRate(int Rate); // Set pin to blinking mode at rate measured in Rate/s
	void setEnable(boolean enable){_enabled = enable;};// Hard enable change
	
	int getState(){return _CurrentValue;};// Return current state
	
	void refresh();//Refresh the pin state - must call often for pin fade/blink effects
	
	void toggle();//Toggle pin state
};

extern HW_PinClass HW_Pin;

#endif

