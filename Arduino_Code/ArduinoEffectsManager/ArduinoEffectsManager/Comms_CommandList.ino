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
void debug_LEDState()
{
	boolean newState = USBSerial.parseInt(PacketPosition1);
	
	TeensyLED.setPin(newState);
	
	Serial.print("Teensy LED State Change;");
	Serial.println(TeensyLED.getState()? "On" : "Off");
}


//LED Manager for multiple LEDs State command
void LEDManager_State()
{
	int _LEDChoice = USBSerial.parseInt(PacketPosition1);
	int _enable = USBSerial.parseInt(PacketPosition2);
	int _brightness = USBSerial.parseInt(PacketPosition3);
	
	Serial.print("LED Value Change, LED Selected;"); Serial.print(_LEDChoice);
	Serial.print(", enable;"); Serial.print(_enable);
	Serial.print(", brightness;"); Serial.println(_brightness);
	
	switch(_LEDChoice)
	{
		case 1: HouseTopLED.setEnable(_enable); HouseTopLED.setAnalogueValue(_brightness); break;
		case 2: HouseBottomLED.setEnable(_enable); HouseBottomLED.setAnalogueValue(_brightness); break;
		case 3: Street1LED.setEnable(_enable); Street1LED.setAnalogueValue(_brightness); break;
		case 4: Street2LED.setEnable(_enable); Street2LED.setAnalogueValue(_brightness); break;
	}
	
	
}
