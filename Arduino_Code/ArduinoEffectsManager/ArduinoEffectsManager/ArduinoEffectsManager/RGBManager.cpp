// 
// 
// 

#include "RGBManager.h"

void RGBManagerClass::init()
{


}

//General setup of class with pin defines for RGB led output.
void RGBManagerClass::setup(int pinR, int pinG, int pinB)
{
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
		analogWrite(_pinR, _r);
		analogWrite(_pinG, _g);
		analogWrite(_pinB, _b);
	}else //Turn off LEDs
	{
		analogWrite(_pinR, 0);
		analogWrite(_pinG, 0);
		analogWrite(_pinB, 0);
	}
}
RGBManagerClass RGBManager;

