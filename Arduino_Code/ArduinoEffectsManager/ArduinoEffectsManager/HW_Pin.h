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

class HW_PinClass
{
 protected:
	int _PinNum;
	int _CurrentValue;
	int _analogueValue;
	boolean _PWMEnable = false;
	int _BlinkRate = 0;
	unsigned long _lastBlink = 0;
	boolean _BlinkEnable = false;
	boolean _enabled = true;

 public:
	void setup(int PinNumber); //Plain setup for easy on/off toggling of pin
	void setup(int PinNumber, boolean PWMEnable); //PWM enable setup for allowing PWM usage
 
	void setPin(int Value); //Set pin state digital
	void setAnalogueValue(int analogueSet); //Set pin state PWM
	void setBlinkRate(int Rate); // Set pin to blinking mode at rate measured in Rate/s
	void setEnable(boolean enable){_enabled = enable;};
	
	int getState(){return _CurrentValue;};// Return current state
	
	void refresh();//Refresh the pin state
	
	void toggle();//Toggle pin state
	
	void init();
};

extern HW_PinClass HW_Pin;

#endif

