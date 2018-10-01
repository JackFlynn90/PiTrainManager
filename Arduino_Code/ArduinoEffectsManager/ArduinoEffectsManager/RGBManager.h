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
	
	boolean _isEnable = true;
 public:
	void init();
	void setup(int pinR, int pinG, int pinB);//General setup call
	void Enable(){_isEnable = true;};//Enable output;
	void Disable(){_isEnable = false;};//Disable output
	void Refresh(); // Refresh latest values for PWM and Enable. Run Often.
	void SetColours(byte r, byte g, byte b){_r = r; _g = g, _b = b;};//set colour values
	
};

extern RGBManagerClass RGBManager;

#endif

