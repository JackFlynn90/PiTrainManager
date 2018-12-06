// 
// 
// 

#include "ServoManager.h"

void ServoManagerClass::setupi2c(Adafruit_PWMServoDriver *i2cDriver, int *DeviceAddress)
{
	
	_i2cDriver = i2cDriver;
	
	_ServoAddress = DeviceAddress[0];
	_LEDGreenAddress = DeviceAddress[1];
	_LEDRedAddress = DeviceAddress[2];

}

void ServoManagerClass::refresh()
{
	if(_Enable)
	{
		if(_CurrentPosition != _TargetPosition)
		{
			_CurrentPosition = _TargetPosition;
			
			if(_CurrentPosition == ServoPosClose)	
				close();
			else
				open();
		}
	}
}

void ServoManagerClass::close()
{
	_i2cDriver->setPWM(_ServoAddress,0,_MinServoPos);
	_i2cDriver->setPWM(_LEDRedAddress,0,_LEDOnPulseLength);
	_i2cDriver->setPWM(_LEDGreenAddress,0,4096);
}

void ServoManagerClass::open()
{
	_i2cDriver->setPWM(_ServoAddress,0,_MaxServoPos);
	_i2cDriver->setPWM(_LEDGreenAddress,0,_LEDOnPulseLength);
	_i2cDriver->setPWM(_LEDRedAddress,0,4096);
	
}

ServoManagerClass ServoManager;

