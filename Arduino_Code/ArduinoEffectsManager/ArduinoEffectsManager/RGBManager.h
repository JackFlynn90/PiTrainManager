// RGBManager.h

#ifndef _RGBMANAGER_h
#define _RGBMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "FastLED/FastLED.h"

#define LED_PIN     17
#define Strip_NUM_LEDS    144
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS  255

class RGBManagerClass
{
 protected:
	byte _r, _g, _b;
	
	CRGB leds[Strip_NUM_LEDS];
	
	int _pinR;
	int _pinG;
	int _pinB;
	
	boolean _isFastLED = false; //New support for RGB FastLED lib
	int _numLEDs;
	
	//Fading effect values used to create a proportional error method of fading LED colours
	byte _Targetr, _Targetg, _Targetb;
	float _FadeRate = 50;
	
	boolean _isEnable = true;
	
	void setStripColour();// Set latest colour to strip and refresh
	void setPinColour();// Set latest colour for single PWM RGB via pins
	
 public:
	void init();
	void setup(int pinR, int pinG, int pinB, float FadeRate);//General setup call
	void setup(int LEDPIN, int Brightness, float FadeRate);//Setup for FastLED lib function
	void Enable(){_isEnable = true;};//Enable output;
	void Disable(){_isEnable = false;};//Disable output
	void Refresh(); // Refresh latest values for PWM and Enable. Run Often for fading effect.
	void SetColours(byte r, byte g, byte b){_Targetr = r; _Targetg = g, _Targetb = b;};//set colour values
	
};

extern RGBManagerClass RGBManager;

#endif

