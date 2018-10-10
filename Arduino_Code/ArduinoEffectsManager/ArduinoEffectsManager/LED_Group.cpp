// 
// 
// 

#include "LED_Group.h"

//General initial setup for class
void LED_GroupClass::setup(int *PinNumbers, int *PWMValues, int numberofPins)
{
	_numPins = numberofPins;
	
	for (int i = 0; i < numberofPins; i++)
	{
	_PinNums[i] = PinNumbers[i];
	_PWMValues[i] = PWMValues[i];
	
	_StoredHWPins[i].setup(PinNumbers[i],true);
	_StoredHWPins[i].setAnalogueValue(PWMValues[i]);
	}
}

//Refresh updates latest pin values with class stored values
void LED_GroupClass::refresh()
{
	for (int i = 0; i < _numPins; i++)
	{
		_StoredHWPins[i].setEnable(_GroupEnable);
		_StoredHWPins[i].setAnalogueValue(_GroupBrightness);
		_StoredHWPins[i].refresh();
	}
}
