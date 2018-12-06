// 
// 
// 

#include "PhotoResistorTrigger.h"

void PhotoResistorTriggerClass::setup(byte pinNumber, int TriggerThreshold)
{
	_pinNumber = pinNumber;
	_triggerValue = TriggerThreshold;
	pinMode(_pinNumber, INPUT_PULLUP);
}

//Photresistor Update
// Updates local variables for triggering
boolean PhotoResistorTriggerClass::upDate()
{
	_currentValue = analogRead(_pinNumber);
	
	if(_currentValue > _triggerValue)
		_triggerOn = true;
	else
		_triggerOn = false;
		
	return _triggerOn;
}





