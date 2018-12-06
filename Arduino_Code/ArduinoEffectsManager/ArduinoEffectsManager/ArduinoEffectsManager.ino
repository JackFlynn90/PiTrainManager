#include "Project_Defines.h"

/*
* ArduinoEffectsManager.ino
*
* Created: 9/26/2018 4:03:43 PM
* Author: Jack Wells
*/




void loop()
{
	SerialUSBCommands(); //Function for handling USB commands received
	
	//LEDs are currently refreshed on a timer. Allows for USB packets to adjust values and fading effects to take place based on new values
	static unsigned long timer_LEDRefresh = millis();
	if(millis() - timer_LEDRefresh > 50)
	{

		RGBManager.Refresh();
		_StreetLights.refresh();
		_HouseLights.refresh();
		
		AutoManager_LDR_Block1.refresh();//Automation testing of using Photoresistor (LDR) to trigger LEDs on off
		
		timer_LEDRefresh = millis();
			
	}
	

	//Debug output to check for USB data flow on the Pi
	static unsigned long timer_handshake = millis();
	if(millis() - timer_handshake > 15000)
	{
		Serial.println("USB Handshake");
		timer_handshake = millis();
	}
	
}




