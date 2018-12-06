// PhotoResistorTrigger.h

#ifndef _PHOTORESISTORTRIGGER_h
#define _PHOTORESISTORTRIGGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*
Simple photoresistor class for handling photoresistor settings and state.

Can be configured to produce a flag when a connected photo resistor passes a given threshold
*/
class PhotoResistorTriggerClass
{
 protected:
	boolean _triggerOn = false;
	int _triggerValue = 512;
	byte _pinNumber;
	int _currentValue;
		
 public:
	void init();
	void setup(byte pinNumber, int TriggerThreshold); //General setup of pin assignment and threshold
	boolean upDate(); //Refresh current states of group LEDs
	boolean getState(){return _triggerOn;};
	int getValue(){return _currentValue;};
	
	
};

#endif

