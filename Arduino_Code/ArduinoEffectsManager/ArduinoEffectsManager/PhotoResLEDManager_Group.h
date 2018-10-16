// PhotoResLEDManager_Group.h

#ifndef _PHOTORESLEDMANAGER_GROUP_h
#define _PHOTORESLEDMANAGER_GROUP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "PhotoResistorTrigger.h"
#include "LED_Group.h"

class PhotoResLEDManager_GroupClass
{
 protected:
	PhotoResistorTriggerClass _LDR;
	LED_GroupClass *_LEDs[20];
	int _numLEDs;
	boolean _LDRState = false;
	boolean _isEnabled = true;
	
 public:
	void init();
	void setup(PhotoResistorTriggerClass LDR, LED_GroupClass *LEDGroupList[], int NumberOfLEDGroups);//Setup passes main LDR sensor and related LED groupings for LDR location
	void refresh();// Handles LDR reading and updates LED enable state based on LDR results
	void setEnable(boolean newState){_isEnabled = newState;};
	boolean getEnable(){return _isEnabled;};
};


#endif

