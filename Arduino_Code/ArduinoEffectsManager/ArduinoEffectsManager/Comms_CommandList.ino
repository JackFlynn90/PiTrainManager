//Main Serial comms function. Checks for incoming data and parses accordingly
void SerialUSBCommands()
{
	//USB Data handling code
	if(USBSerial.Read())
	{
		switch(USBSerial.parseCommand()) //Parses packet for "Command" from first position. This launches the appropriate function based on the command received
		{
			case 1: Command_RGBHex_Received(); break; //RGB Colour Handling
			case 2: Command_RGBOutput_state(); break; //RGB on/off state
			case 3: Command_debug_LEDState(); break; // Debug LED on/off state
			case 4: Command_LEDManager_State();break; // Group LED on/off states
			case 5: Command_LEDManager_Brightness();break; // Grouping LED brightness levels
			case 6: Command_LDRBlockManager_State();break; //LDR grouping manager
			case 7: Command_ServoPosition();break; //Servo position change command
			
			default: Serial.println("Command Not Recognised. Data received;"); USBSerial.printLastPacket(); break; //Fall back exception
		}
	}
	
}

//RGB Hex value command
// Changes stored colour values for RGB Manager class
void Command_RGBHex_Received()
{
	byte hexVal[3] = {(byte)USBSerial.parseInt(PacketPosition1),(byte)USBSerial.parseInt(PacketPosition2),(byte)USBSerial.parseInt(PacketPosition3)};
	
	Serial.print("Hex value received;");
	for (int i = 0; i <3; i++)
	Serial.print(hexVal[i]);
	Serial.println();
	
	RGBManager.SetColours(hexVal[0], hexVal[1], hexVal[2]);

}

//RGB Output control command
// Enable/Disable the RGB LED output. Passes to RGBManager class
void Command_RGBOutput_state()
{
	boolean enable = USBSerial.parseInt(PacketPosition2);
	Serial.print("RGB Output State;");
	Serial.println(enable? "Enabled" : "Disabled");
	
	if(enable)
	RGBManager.Enable();
	else
	RGBManager.Disable();
}

//Debug LED State command
void Command_debug_LEDState()
{
	boolean newState = USBSerial.parseInt(PacketPosition1);
	
	TeensyLED.setPin(newState);
	
	Serial.print("Teensy LED State Change;");
	Serial.println(TeensyLED.getState()? "On" : "Off");
}


//LED Manager for multiple LEDs State command
// Used to parse which LED group is being effected and update the groups enable state
void Command_LEDManager_State()
{
	int _LEDChoice = USBSerial.parseInt(PacketPosition1);
	int _enable = USBSerial.parseInt(PacketPosition2);
	
	
	Serial.print("LED Group Change, Grouping Selected;"); Serial.print(_LEDChoice);
	Serial.print(", enable;"); Serial.println(_enable);

	
	switch(_LEDChoice)
	{
		case 0: Command_RGBOutput_state(); break;
		case 1: _HouseLights.setAllEnables(_enable); _HouseLights.refresh(); break;
		case 2: _StreetLights.setAllEnables(_enable); _StreetLights.refresh();break;
	}
	
	
}

//LED Manager for multiple LEDs brightness command
// Used to parse which LED group is being effected and update the groups brightness level
void Command_LEDManager_Brightness()
{
	int _LEDChoice = USBSerial.parseInt(PacketPosition1);
	int _brightness = USBSerial.parseInt(PacketPosition2);
	
	Serial.print("LED Group Change, Grouping Selected;"); Serial.print(_LEDChoice);
	Serial.print(", brightness;"); Serial.println(_brightness);
	
	switch(_LEDChoice)
	{
		case 0: Command_RGBOutput_state(); break;
		case 1: _HouseLights.setAllBrightness(_brightness); _HouseLights.refresh(); break;
		case 2: _StreetLights.setAllBrightness(_brightness); _StreetLights.refresh();break;
	}
	
	
}

//LDR block state manager
// Used to control the enable state of the LDR blocks which control the local LED states
void Command_LDRBlockManager_State()
{
	int LDRBlockChoice = USBSerial.parseInt(PacketPosition1);
	boolean newState = USBSerial.parseInt(PacketPosition2);
	
	Serial.print("LDR Block state change, LDR selected;"); Serial.print(LDRBlockChoice);
	Serial.print(", New state;"); Serial.println(newState);
	
	switch (LDRBlockChoice)
	{
		case 0: AutoManager_LDR_Block1.setEnable(newState); AutoManager_LDR_Block1.refresh(); break;
	}
}

//Servo Position Handler
// Used to control the position of servo states on the track
void Command_ServoPosition()
{
	int ServoBlockChoise = USBSerial.parseInt(PacketPosition1);
	boolean newState = USBSerial.parseInt(PacketPosition2);
	
	Serial.print("Servo block state change, Servos selected;"); Serial.print(ServoBlockChoise);
	Serial.print(", New state;"); Serial.println(newState? "Open":"Close");
	
	switch (ServoBlockChoise)
	{
		case 0: _Servo1.setPosition(newState); break;
	}
}