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
HW_PinClass HouseTopLED;
HW_PinClass HouseBottomLED;
HW_PinClass Street1LED;
HW_PinClass Street2LED;

#define pinHouseTopLED 20
#define pinHouseBotLED 3
#define pinStreet1LED 4
#define pinStreet2LED 5


#define pinRGB_r 21
#define pinRGB_g 23
#define pinRGB_b 22

void setup()
{
	RGBManager.setup(pinRGB_r,pinRGB_g,pinRGB_b);
	USBSerial.setup();

	HouseTopLED.setup(pinHouseTopLED, true);
	HouseBottomLED.setup(pinHouseBotLED, true);
	Street1LED.setup(pinStreet1LED, true);
	Street2LED.setup(pinStreet2LED, true);

	
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
			
			default: Serial.println("Command Not Recognised. Data received;"); USBSerial.printLastPacket(); break;
		}
	}

	RGBManager.Refresh();
	
	static unsigned long timer_handshake = millis();
	
	if(millis() - timer_handshake > 15000)
	{
		Serial.println("USB Handshake");
		timer_handshake = millis();
	}

	for (int i = 0; i < 255; i++)
	{
		HouseTopLED.setAnalogueValue(i);
		HouseBottomLED.setAnalogueValue(i);
		Street1LED.setAnalogueValue(i);
		Street2LED.setAnalogueValue(i);
		delay(10);
		Serial.print("Brightness Up;");
		Serial.println(i);
	}

	delay(500);
	
	for (int j = 0; j < 255; j++)
	{
		int i = 255-j;
		
		HouseTopLED.setAnalogueValue(i);
		HouseBottomLED.setAnalogueValue(i);
		Street1LED.setAnalogueValue(i);
		Street2LED.setAnalogueValue(i);
		delay(10);
		
		Serial.print("Brightness Down;");
		Serial.println(255-i);
	}
	
	delay(500);
}


