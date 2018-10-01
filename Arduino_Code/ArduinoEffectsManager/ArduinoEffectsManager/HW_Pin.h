// HW_Pin.h

#ifndef _HW_PIN_h
#define _HW_PIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define LEDON 0
#define LEDOFF 1

class HW_PinClass
{
 protected:
	int _PinNum;

 public:
	void setup(int PinNumber);
	void SetPin(int Value);
	int _Status;
	
	void toggle();//Toggle pin state
	
	void init();
};

extern HW_PinClass HW_Pin;

#endif

