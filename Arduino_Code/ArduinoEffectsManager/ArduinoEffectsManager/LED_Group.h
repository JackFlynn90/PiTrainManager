// LED_Group.h

#ifndef _LED_GROUP_h
#define _LED_GROUP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "HW_Pin.h"

/*
LED Grouping
	Allows for bulk changing of LEDs without single addressing each one.
*/
class LED_GroupClass
{
	protected:
	int _PinNums[20];
	int _PWMValues[20];
	int _Enables[20];
	boolean _GroupEnable = true;
	int _GroupBrightness = 128;
	int _numPins;
	float _FadeRate = 50.0;
	
	HW_PinClass _StoredHWPins[20];
	
	public:
	void setup(int *PinNumbers, int *PWMValues, int numberofPins, float FadeRate); //Plain setup for easy on/off toggling of pin
	
	void setSinglePWM(int GroupAddress, int analogueSet); //Set single pin state PWM - to be updated
	void setSingleEnable(int GroupAddress, int analogueSet); //Set single pin enable state - to be updated
	
	void setAllEnables(boolean Enable){_GroupEnable = Enable;}; //Group setting
	void setAllBrightness(int Brightness){_GroupBrightness = Brightness;}; // group setting
	
	int getState(){return _GroupEnable;};// Return current state
	
	void refresh();//Refresh the current state of the group pins

};

#endif

