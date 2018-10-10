#include "PhotoResistorTrigger.h"
#include "Defines_PinList.h"
#include "LED_Group.h"
#include "RGBManager.h"
/*
* ArduinoEffectsManager.ino
*
* Created: 9/26/2018 4:03:43 PM
* Author: Admin
*/
#include "Comms_SerialExtended.h"
#include "HW_Pin.h"

Comms_SerialExtendedClass USBSerial;

HW_PinClass TeensyLED;

LED_GroupClass StreetLights;
LED_GroupClass HouseLights;

PhotoResistorTriggerClass LDRGroup_Block1; // Photoresistor wrapper class

#define StreetLightFadeRate 500
#define HouseLightFadeRate 50
#define RGBFadeRate 20

void setup()
{
	RGBManager.setup(pinRGB_r,pinRGB_g,pinRGB_b, RGBFadeRate);//RGB LED class for handling RGB single LED colour
	
	USBSerial.setup(); //Custom USB serial comms class for handling incomming packets
	USBSerial._PacketLayout.StartChar = ':';
	USBSerial._PrintIncomming = true; //Debug echo incomming serial data
	
	//LED Group setups. Grouped LEDs allow for easy control of groupings such as "Street lights" or housing
	int numbStreetLights = 2;
	int StreetLightPinList[] = {pinStreet1LED, pinStreet2LED};
	int StreetLightBrightnessList[] = {255,10};
	StreetLights.setup(StreetLightPinList,StreetLightBrightnessList,numbStreetLights, StreetLightFadeRate);
	
	int numbHouseLights = 2;
	int HouseLightsPinList[] = {pinHouseTopLED, pinHouseBotLED};
	int HouseLightsBrightnessList[] = {255,128};
	HouseLights.setup(HouseLightsPinList,HouseLightsBrightnessList,numbHouseLights, HouseLightFadeRate);
	
	//Setup for LDR classes
	LDRGroup_Block1.setup(pin_in_PhotoRes, 700);
	
	//Debug LED setup
	TeensyLED.setup(13);
	TeensyLED.setPin(LOW);
	
	pinMode(pin_in_PhotoRes, INPUT_PULLUP);
	
	Serial.println("Hello World from ArduinoEffectsManager");
	
}

void loop()
{
	//USB Data handling code
	if(USBSerial.Read())
	{
		switch(USBSerial.parseCommand()) //Parses packet for "Command" from first position. This launches the appropriate function based on the command received
		{
			case 1: RGBHex_Received(); break; //RGB Colour Handling
			case 2: RGBOutput_state(); break; //RGB on/off state
			case 3: debug_LEDState(); break; // Debug LED on/off state
			case 4: LEDManager_State();break; // Group LED on/off states
			case 5: LEDManager_Brightness();break; // Grouping LED brightness levels
			
			default: Serial.println("Command Not Recognised. Data received;"); USBSerial.printLastPacket(); break; //Fall back exception
		}
	}
	
	
	//LEDs are currently refreshed on a timer. Allows for USB packets to adjust values and fading effects to take place based on new values
	static unsigned long timer_LEDRefresh = millis();
	if(millis() - timer_LEDRefresh > 50)
	{

		RGBManager.Refresh();
		StreetLights.refresh();
		HouseLights.refresh();
		timer_LEDRefresh = millis();
		LDRTrigger_LEDs();
			
	}
	

	//Debug output to check for USB data flow on the Pi
	static unsigned long timer_handshake = millis();
	if(millis() - timer_handshake > 15000)
	{
		Serial.println("USB Handshake");
		timer_handshake = millis();
	}
	
}

//Automation testing of using Photoresistor (LDR) to trigger LEDs on off
void LDRTrigger_LEDs()
{
	static boolean LDRTrigger = false;
	
	if(LDRTrigger != LDRGroup_Block1.refresh())
	{
		if(LDRGroup_Block1.getState())
		{
			HouseLights.setAllEnables(true);
			StreetLights.setAllEnables(true);
		}else
		{
			HouseLights.setAllEnables(false);
			StreetLights.setAllEnables(false);
		}
		LDRTrigger = LDRGroup_Block1.getState();
		
		Serial.print("LDR Block1 state change. New state;"); Serial.println(LDRGroup_Block1.getState());
	}
	
	
}


