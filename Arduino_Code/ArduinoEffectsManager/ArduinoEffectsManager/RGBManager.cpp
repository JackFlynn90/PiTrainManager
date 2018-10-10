//
//
//

#include "RGBManager.h"

void RGBManagerClass::init()
{


}

//General setup of class with pin defines for RGB led output.
void RGBManagerClass::setup(int pinR, int pinG, int pinB, float FadeRate)
{
	_FadeRate = FadeRate;
	
	_pinR = pinR;
	_pinG = pinG;
	_pinB = pinB;
	
	pinMode(OUTPUT, _pinR);
	pinMode(OUTPUT, _pinG);
	pinMode(OUTPUT, _pinB);
}

//Refresh RGB status. Call often to ensure no missing changes
void RGBManagerClass::Refresh()
{
	if(_isEnable)//PWM LEDs
	{
		_FadeR = _FadeR + (_Targetr - _FadeR)/_FadeRate;
		_r = _FadeR;
		_FadeB = _FadeB + (_Targetb - _FadeB)/_FadeRate;
		_b = _FadeB;
		_FadeG = _FadeG + (_Targetg - _FadeG)/_FadeRate;
		_g = _FadeG;
		
		analogWrite(_pinR, _r);
		analogWrite(_pinG, _g);
		analogWrite(_pinB, _b);
	}else //Turn off LEDs
	{
		_FadeR = 0;
		_FadeG = 0;
		_FadeB = 0;
		analogWrite(_pinR, 0);
		analogWrite(_pinG, 0);
		analogWrite(_pinB, 0);
	}
}
RGBManagerClass RGBManager;

