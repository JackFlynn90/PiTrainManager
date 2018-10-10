//
//
//

#include "HW_Pin.h"

void HW_PinClass::init()
{


}

//Basic digital setup
// Defaults pin state to false
void HW_PinClass::setup(int PinNum)
{
	_PinNum = PinNum;
	_CurrentValue = false;
	
	pinMode(_PinNum, OUTPUT);
	
	digitalWriteFast(_PinNum, _CurrentValue);
}

//Basic digital setting of pin value
void HW_PinClass::setPin(int Value)
{
	_CurrentValue = Value;
	digitalWriteFast(_PinNum,_CurrentValue);
}

//Basic digital flipping of pin state
void HW_PinClass::toggle()
{
	_CurrentValue = !_CurrentValue;
	digitalWriteFast(_PinNum,_CurrentValue);
}

//Analogue Pin enable
// -Doesn't currently check if pin has analogue support
void HW_PinClass::setup(int PinNumber, boolean PWMEnable, float FadeRate)
{
	_FadeRate = FadeRate;
	_PinNum = PinNumber;
	_CurrentValue = 0;
	_PWMEnable = PWMEnable;
	pinMode(_PinNum, OUTPUT);
	analogWrite(_PinNum,_CurrentValue);
}

//Analogue write value
// relies on hardware PWM analogue write
void HW_PinClass::setAnalogueValue(int analogueSet)
{
	if(_FadeEnable)
	_FadetargetValue = analogueSet;
	else
	{
		_CurrentValue = analogueSet;
		analogWrite(_PinNum,_CurrentValue);
	}
}

//Digital blink rate
// Allows for easy blinking of LED
void HW_PinClass::setBlinkRate(int Rate)
{
	_BlinkEnable = true;
	_BlinkRate = 1000/Rate;
}

//Refresh function handles all possible pin states and updates accordingly
void HW_PinClass::refresh()
{
	if(_enabled)
	{
		if(_BlinkEnable)
		{
			if(millis() - _lastBlink > _BlinkRate)
			{
				_CurrentValue = !_CurrentValue;
				digitalWriteFast(_PinNum,_CurrentValue);
				_lastBlink = millis();
			}
		}else
		{
			if(_PWMEnable)
			{
				if(_FadeEnable)
				{
					_FadeValue = _FadeValue + (_FadetargetValue - _FadeValue)/_FadeRate;
					_CurrentValue = _FadeValue;	
				}
				
				analogWrite(_PinNum, _CurrentValue);
				
			}else
			{
				digitalWriteFast(_PinNum,_CurrentValue);
			}
			
		}
	}else
	{
		_FadeValue = 0;
		
		if(_PWMEnable)
		analogWrite(_PinNum,0);
		else
		digitalWriteFast(_PinNum, 0);
	}
}
HW_PinClass HW_Pin;

