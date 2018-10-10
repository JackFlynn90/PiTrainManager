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

#define StreetLightFadeRate 500
#define HouseLightFadeRate 50
#define RGBFadeRate 20

void setup()
{
	RGBManager.setup(pinRGB_r,pinRGB_g,pinRGB_b, RGBFadeRate);
	USBSerial.setup();

	
	int numbStreetLights = 2;
	int StreetLightPinList[] = {pinStreet1LED, pinStreet2LED};
	int StreetLightBrightnessList[] = {255,10};
	
	StreetLights.setup(StreetLightPinList,StreetLightBrightnessList,numbStreetLights, StreetLightFadeRate);
	
	int numbHouseLights = 2;
	int HouseLightsPinList[] = {pinHouseTopLED, pinHouseBotLED};
	int HouseLightsBrightnessList[] = {255,128};
	
	HouseLights.setup(HouseLightsPinList,HouseLightsBrightnessList,numbHouseLights, HouseLightFadeRate);
	
	
	TeensyLED.setup(13);
	TeensyLED.setPin(LOW);
	
	USBSerial._PacketLayout.StartChar = ':';
	
	Serial.println("Hello World from ArduinoEffectsManager");
	USBSerial._PrintIncomming = true;
}

void loop()
{
	if(USBSerial.Read())
	{
		switch(USBSerial.parseCommand())
		{
			case 1: RGBHex_Received(); break;
			case 2: RGBOutput_state(); break;
			case 3: debug_LEDState(); break;
			case 4: LEDManager_State();break;
			case 5: LEDManager_Brightness();break;
			
			default: Serial.println("Command Not Recognised. Data received;"); USBSerial.printLastPacket(); break;
		}
	}
	
	
	static unsigned long timer_LEDRefresh = millis();
	if(millis() - timer_LEDRefresh > 50)
	{

		RGBManager.Refresh();
		StreetLights.refresh();
		HouseLights.refresh();
		timer_LEDRefresh = millis();
	}
	
	static unsigned long timer_handshake = millis();
	
	if(millis() - timer_handshake > 15000)
	{
		Serial.println("USB Handshake");
		timer_handshake = millis();
	}

}


