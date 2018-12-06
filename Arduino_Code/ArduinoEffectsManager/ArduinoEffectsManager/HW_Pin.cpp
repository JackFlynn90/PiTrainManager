//
//
//

#include "HW_Pin.h"

//******************************************************************
//Basic digital setup
// Defaults pin state to false
void HW_PinClass::setup(byte PinNum)
{
	_PinNum = PinNum;
	_CurrentValue = false;
	
	pinMode(_PinNum, OUTPUT);
	
	digitalWriteFast(_PinNum, _CurrentValue);
}

//******************************************************************
//Basic digital setting of pin value
void HW_PinClass::setPin(byte Value)
{
	_CurrentValue = Value;
	digitalWriteFast(_PinNum,_CurrentValue);
}

//******************************************************************
//Basic digital flipping of pin state
void HW_PinClass::toggle()
{
	_CurrentValue = !_CurrentValue;
	digitalWriteFast(_PinNum,_CurrentValue);
}

//******************************************************************
//Analogue Pin Setup
// -Doesn't currently check if pin has analogue support
void HW_PinClass::setup(byte PinNumber, boolean PWMEnable, float FadeRate)
{
	_FadeRate = (byte)FadeRate;
	_PinNum = PinNumber;
	_CurrentValue = 0;
	_PWMEnable = PWMEnable;
	pinMode(_PinNum, OUTPUT);
	analogWrite(_PinNum,_CurrentValue);
}

//******************************************************************
//Analogue write value
// Relies on hardware PWM analogue write
// When fading is used it sets the target fade value which is reached during refresh()
void HW_PinClass::setAnalogueValue(byte analogueSet)
{
	if(_FadeEnable)
	_FadetargetValue = analogueSet;
	else
	{
		_CurrentValue = analogueSet;
		analogWrite(_PinNum,_CurrentValue);
	}
}


//******************************************************************
//Digital blink rate
// Allows for easy blinking of LED
void HW_PinClass::setBlinkRate(byte Rate)
{
	_BlinkEnable = true;
	_BlinkRate = 1000/Rate;
}

//******************************************************************
//Refresh function handles all possible pin states and updates accordingly
void HW_PinClass::refresh()
{
	if(_enabled) //Hard on/off state
	{
		if(_BlinkEnable)
		{
			if(millis() - _lastBlink > _BlinkRate) //Timer used for blinking
			{
				_CurrentValue = !_CurrentValue;
				digitalWriteFast(_PinNum,_CurrentValue);
				_lastBlink = millis();
			}
		}else
		{
			if(_PWMEnable)// PWM analogue usage
			{
				if(_FadeEnable) //Fading effect achieved by moving current value towards the target value at "fade rate"
				{
					_FadeValue = _FadeValue + (_FadetargetValue - _FadeValue)/_FadeRate;
					_CurrentValue = _FadeValue;	
				}
				
				analogWrite(_PinNum, _CurrentValue);
				
			}else //Plain digital on/off
			{
				digitalWriteFast(_PinNum,_CurrentValue);
			}
			
		}
	}else // Hard off state
	{
		_FadeValue = 0;
		
		if(_PWMEnable)
		analogWrite(_PinNum,0);
		else
		digitalWriteFast(_PinNum, 0);
	}
}
HW_PinClass HW_Pin;

