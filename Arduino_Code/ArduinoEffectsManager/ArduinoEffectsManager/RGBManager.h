// RGBManager.h

#ifndef _RGBMANAGER_h
#define _RGBMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class RGBManagerClass
{
 protected:
	byte _r, _g, _b;
	
	int _pinR;
	int _pinG;
	int _pinB;
	
	//Fading effect values used to create a proportional error method of fading LED colours
	byte _Targetr, _Targetg, _Targetb;
	float _FadeRate = 50;
	float _FadeR, _FadeG, _FadeB;
	
	boolean _isEnable = true;
 public:
	void init();
	void setup(int pinR, int pinG, int pinB, float FadeRate);//General setup call
	void Enable(){_isEnable = true;};//Enable output;
	void Disable(){_isEnable = false;};//Disable output
	void Refresh(); // Refresh latest values for PWM and Enable. Run Often for fading effect.
	void SetColours(byte r, byte g, byte b){_Targetr = r; _Targetg = g, _Targetb = b;};//set colour values
	
};

extern RGBManagerClass RGBManager;

#endif

