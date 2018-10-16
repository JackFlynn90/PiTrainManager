//
//
//

#include "PhotoResLEDManager_Group.h"

//Setup passes main LDR sensor and related LED groupings for LDR location
void PhotoResLEDManager_GroupClass::setup(PhotoResistorTriggerClass LDR, LED_GroupClass *LEDGroupList[], int NumberOfLEDGroups)
{
	
	_numLEDs = NumberOfLEDGroups;
	
	for(int i = 0; i < _numLEDs; i++)
	_LEDs[i] = LEDGroupList[i];

	/*	_LEDs = *LEDGroupList;*/
	LDR = _LDR;

}

//Refresh
// Handles LDR reading and updates LED enable state based on LDR results
void PhotoResLEDManager_GroupClass::refresh()
{
	if(_isEnabled)
	{
		if(_LDRState != _LDR.upDate())
		{
			if(_LDR.getState())
			{
				for(int i = 0; i < _numLEDs; i++)
				_LEDs[i]->setAllEnables(true);
			}else
			{
				for(int i = 0; i < _numLEDs; i++)
				_LEDs[i]->setAllEnables(false);
			}
			_LDRState = _LDR.getState();
			
			Serial.print("LDR state change. New state;"); Serial.println(_LDR.getState());
		}
	}
}


PhotoResLEDManager_GroupClass PhotoResLEDManager_Group;

