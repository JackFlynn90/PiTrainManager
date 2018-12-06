// ServoManager.h

#ifndef _SERVOMANAGER_h
#define _SERVOMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "AdafruitPWMLib/Adafruit_PWMServoDriver.h"

#define ServoPosOpen 1
#define ServoPosClose 0

class ServoManagerClass
{
	protected:
	Adafruit_PWMServoDriver *_i2cDriver;
	
	int _ServoAddress;
	int _CurrentPosition; //Current PWM Value
	int _TargetPosition; //Fade Target value for PWM mode
	int _Enable = true;
	
	int _LEDGreenAddress;
	int _LEDRedAddress; 
	
	int _MaxServoPos = 600;
	int _MinServoPos = 150;
	int _LEDOnPulseLength = 512;
	
	void close(); // Close servo position, light leds
	void open(); // open servo position, light leds
	
	public:
	
	void setPosition(int NewPosition) {_TargetPosition = NewPosition;}; //Set servo target position
	
	void setAllEnables(boolean Enable){_Enable = Enable;}; //Group setting
	
	int getState(){return _Enable;};// Return current state
	
	void refresh();//Refresh the current state of the servo
	
	void setupi2c(Adafruit_PWMServoDriver *i2cDriver, int *DeviceAddress); // Setup for PWM servo board

};

extern ServoManagerClass ServoManager;

#endif

