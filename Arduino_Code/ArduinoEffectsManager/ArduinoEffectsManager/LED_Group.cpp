//
//
//

#include "LED_Group.h"

//General initial setup for class
void LED_GroupClass::setup(int *PinNumbers, int *PWMValues, int numberofPins, float FadeRate)
{
	_FadeRate = FadeRate;
	
	_numPins = numberofPins;
	
	for (int i = 0; i < numberofPins; i++)
	{
		_PinNums[i] = PinNumbers[i];
		_PWMTargetValues[i] = PWMValues[i];
		
		_StoredHWPins[i].setup(PinNumbers[i],true, _FadeRate);
		_StoredHWPins[i].setAnalogueValue(_PWMTargetValues[i]);
	}
}

//General initial setup for class
void LED_GroupClass::setupi2c(Adafruit_PWMServoDriver *i2cDriver, int *LedAddress, int *PWMValues, int numberofLEDs, float FadeRate)
{
	_isi2c = true;
	
	_FadeRate = FadeRate;
	
	_numPins = numberofLEDs;
	
	_i2cDriver = i2cDriver;
	
	for (int i = 0; i < numberofLEDs; i++)
	{
		_PinNums[i] = LedAddress[i];
		_PWMTargetValues[i] = PWMValues[i];
		_PWMCurrentValues[i] = 0;
	}
	
}

//Refresh updates latest pin values with class stored values
void LED_GroupClass::refresh()
{
	if(_isi2c)
	{
		if(_GroupEnable)
		i2cGroupSetValue(_GroupBrightness);
		else
		i2cGroupSetValue(0);
		
		i2cGroupRefresh();
		
	}
	else
	{
		for (int i = 0; i < _numPins; i++)
		{
			_StoredHWPins[i].setEnable(_GroupEnable);
			_StoredHWPins[i].setAnalogueValue(_GroupBrightness);
			_StoredHWPins[i].refresh();
		}
	}
}



//Set whole group to use PWM target
void LED_GroupClass::i2cGroupSetValue(int PWMValue)
{
	for (int i = 0; i < _numPins; i++)
	{
		_PWMTargetValues[i] = PWMValue;
	}
}

//Cycle through group setting PWM state using fading effects
void LED_GroupClass::i2cGroupRefresh()
{
	
	for (int i = 0; i < _numPins; i++)
	{
		if(_FadeEnable) //Fading effect achieved by moving current value towards the target value at "fade rate"
		{
			if(_PWMCurrentValues[i] != _PWMTargetValues[i])
			{
				int FadeValue = _PWMCurrentValues[i] + (_PWMTargetValues[i] -  _PWMCurrentValues[i])/_FadeRate;
				_PWMCurrentValues[i] = FadeValue;
			}
			
			_i2cDriver->setPWM(_PinNums[i], _PWMCurrentValues[i]*4, 4096); //write faded PWM values to LEDs
			
		}else
		_i2cDriver->setPWM(_PinNums[i], _PWMCurrentValues[i]*4, 4096); // write PWM straight values to LEDs
		
	}
}