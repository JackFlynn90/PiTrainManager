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
	
#define pinRGB_r 21
#define pinRGB_g 23
#define pinRGB_b 22

void setup()
{
	RGBManager.setup(pinRGB_r,pinRGB_g,pinRGB_b);
	USBSerial.setup();
	
	TeensyLED.setup(13);
	TeensyLED.SetPin(LOW);
	
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
			case 3: LEDState(); break;
			
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
	

}

//RGB Hex value command
// Changes stored colour values for RGB Manager class
void RGBHex_Received()
{
	byte hexVal[3] = {USBSerial.parseInt(PacketPosition1),USBSerial.parseInt(PacketPosition2),USBSerial.parseInt(PacketPosition3)};
	
	Serial.print("Hex value received;");
	for (int i = 0; i <3; i++)
	Serial.print(hexVal[i]);
	Serial.println();
	
	RGBManager.SetColours(hexVal[0], hexVal[1], hexVal[2]);

}

//RGB Output control command
// Enable/Disable the RGB LED output. Passes to RGBManager class
void RGBOutput_state()
{	
		boolean enable = USBSerial.parseInt(PacketPosition1);
		Serial.print("RGB Output State;");
		Serial.println(enable? "Enabled" : "Disabled");
		
		if(enable)
		RGBManager.Enable();
		else
		RGBManager.Disable();
}

//LED State command
void LEDState()
{
	boolean newState = USBSerial.parseInt(PacketPosition1);
	
	TeensyLED.SetPin(newState);
	
	Serial.print("Teensy LED State Change;");
	Serial.println(TeensyLED._Status? "On" : "Off");
}