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

//Fast LED Setup
void RGBManagerClass::setup(int LEDPIN, int Brightness, float FadeRate)
{
	_FadeRate = FadeRate;
	_isFastLED = true;
	_numLEDs = Strip_NUM_LEDS;
	LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, Strip_NUM_LEDS);
	FastLED.setBrightness(Brightness);
}

//Refresh RGB status. Call often to ensure no missing changes
void RGBManagerClass::Refresh()
{
	
	if(_isEnable)//Fade into Target colour
	{
		_r = _r + (_Targetr - _r)/_FadeRate;

		_b = _b + (_Targetb - _b)/_FadeRate;

		_g = _g + (_Targetg - _g)/_FadeRate;

	}else //Turn off LEDs
	{
		_r = 0;
		_b = 0;
		_g = 0;
	}
	
	if(_isFastLED)
	setStripColour();
	else
	setPinColour();
}

//FastLED colour update based on RGB values
void RGBManagerClass::setStripColour()
{
	for(int i =0; i < _numLEDs; i++)
	leds[i].setRGB(_r,_g,_b);
	
	FastLED.show();
}

//Simple pin state based on RGB values via PWM writing
void RGBManagerClass::setPinColour()
{
	analogWrite(_pinR, _r);
	analogWrite(_pinG, _g);
	analogWrite(_pinB, _b);
}
RGBManagerClass RGBManager;

