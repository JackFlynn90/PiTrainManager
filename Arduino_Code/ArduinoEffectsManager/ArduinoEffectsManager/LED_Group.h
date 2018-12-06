// LED_Group.h

#ifndef _LED_GROUP_h
#define _LED_GROUP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include "HW_Pin.h"
#include "AdafruitPWMLib/Adafruit_PWMServoDriver.h"

/*
LED Grouping
	Allows for bulk changing of LEDs without single addressing each one.
	Array for StoredHWPins class is default to 20 held pins. Not likely to require more
	however eventually this class will be updated to handle buffer chips for more complex
	led control
	
change 06/12/18
	Code updated to support Adafruit PWM i2c LED driver with 16 channels
	
*/

#define MaxNoLEDs 16
class LED_GroupClass
{
	protected:
	Adafruit_PWMServoDriver *_i2cDriver;
	
	byte _PinNums[MaxNoLEDs];
	byte _PWMCurrentValues[MaxNoLEDs]; //Current PWM Value
	byte _PWMTargetValues[MaxNoLEDs]; //Fade Target value for PWM mode
	byte _Enables[MaxNoLEDs];
	boolean _GroupEnable = true;
	byte _GroupBrightness = 128;
	byte _numPins;
	float _FadeRate = 50.0;
	boolean _FadeEnable = true; // Flag for fading
	
	HW_PinClass _StoredHWPins[MaxNoLEDs];
	
	boolean _isi2c = false; //flag for using i2c servo board
	
	//Set whole group brightness value
	void i2cGroupSetValue(byte PWMValue);
	//Refresh LED pin values
	void i2cGroupRefresh();
	
	public:
	void setup(byte *PinNumbers, byte *PWMValues, byte numberofPins, float FadeRate); //Plain setup for easy on/off toggling of pin
	
	void setSinglePWM(byte LEDAddress, byte analogueSet) {_PWMTargetValues[LEDAddress] = analogueSet;}; //Set single pin state PWM - to be updated
	void setSingleEnable(byte LedAddress, byte analogueSet); //Set single pin enable state - to be updated
	
	void setAllEnables(boolean Enable){_GroupEnable = Enable;}; //Group setting
	void setAllBrightness(byte Brightness){_GroupBrightness = Brightness;}; // group setting
	
	int getState(){return _GroupEnable;};// Return current state
	
	void refresh();//Refresh the current state of the group pins
	
	void setupi2c(Adafruit_PWMServoDriver *i2cDriver,const byte *LedAddress, byte *PWMValues,const byte numberofLEDs, float FadeRate); // New setup for PWM servo board handling LEDs

};

#endif

