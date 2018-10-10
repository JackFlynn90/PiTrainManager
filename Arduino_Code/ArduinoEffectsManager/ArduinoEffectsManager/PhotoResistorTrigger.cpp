// 
// 
// 

#include "PhotoResistorTrigger.h"

void PhotoResistorTriggerClass::setup(int pinNumber, int TriggerThreshold)
{
	_pinNumber = pinNumber;
	_triggerValue = TriggerThreshold;
	pinMode(_pinNumber, INPUT_PULLUP);
}

boolean PhotoResistorTriggerClass::refresh()
{
	_currentValue = analogRead(_pinNumber);
	
	if(_currentValue > _triggerValue)
		_triggerOn = true;
	else
		_triggerOn = false;
		
	return _triggerOn;
}


PhotoResistorTriggerClass PhotoResistorTrigger;

