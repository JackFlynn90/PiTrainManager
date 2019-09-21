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
	
	Serial.print(F("Hex value received;"));
	for (byte i = 0; i <3; i++)
	Serial.print(hexVal[i]);
	Serial.println();
	
	RGBManager.SetColours(hexVal[0], hexVal[1], hexVal[2]);

}

//RGB Output control command
// Enable/Disable the RGB LED output. Passes to RGBManager class
void Command_RGBOutput_state()
{
	boolean enable = USBSerial.parseInt(PacketPosition2);
	Serial.print(F("RGB Output State;"));
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
	
	Serial.print(F("Teensy LED State Change;"));
	Serial.println(TeensyLED.getState()? "On" : "Off");
}


//LED Manager for multiple LEDs State command
// Used to parse which LED group is being effected and update the groups enable state
void Command_LEDManager_State()
{
	byte _LEDChoice = USBSerial.parseInt(PacketPosition1);
	byte _boardAdd = USBSerial.parseInt(PacketPosition2);
	byte _brightness = USBSerial.parseInt(PacketPosition3);
	byte _enable = USBSerial.parseInt(4);
	
	
	Serial.print(F("LED Single Change, Selected pin;")); Serial.println(_LEDChoice);
	Serial.print(F("LED Single Change, Selected board;")); Serial.println(_boardAdd);
	Serial.print(F("LED Single Change, brightness ;")); Serial.println(_brightness);
	Serial.print(F(", enable;")); Serial.println(_enable? "On" : "Off");

	
	switch(_boardAdd)
	{
		case 0: cycleLEDs(); break;
		case 1: if(_enable)_LEDDriver1.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver1.setPWM(_LEDChoice, 0, 4095);break;
		case 2: if(_enable)_LEDDriver2.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver2.setPWM(_LEDChoice, 0, 4095);break;
		case 3: if(_enable)_LEDDriver3.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver3.setPWM(_LEDChoice, 0, 4095);break;
		case 4: if(_enable)_LEDDriver4.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver4.setPWM(_LEDChoice, 0, 4095);break;
		case 5: if(_enable)_LEDDriver5.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver5.setPWM(_LEDChoice, 0, 4095);break;
	}
	
	
}

void cycleLEDs()
{
		int state = 128;
		while(true)
		{
			
			for (int i = 0; i < 16; i++)
			{
				_LEDDriver1.setPWM(i, state*8, 4095);
				delay(250);
			}
			
			for (int i = 0; i < 16; i++)
			{
				_LEDDriver2.setPWM(i, state*8, 4095);
				delay(250);
			}
			

			for (int i = 0; i < 16; i++)
			{
				_LEDDriver3.setPWM(i, state*8, 4095);
				delay(250);
			}
			
			for (int i = 0; i < 16; i++)
			{
				_LEDDriver4.setPWM(i, state*8, 4095);
				delay(250);
			}
			
			for (int i = 0; i < 16; i++)
			{
				_LEDDriver5.setPWM(i, state*8, 4095);
				delay(250);
			}
			
			
			if(state == 128)
			state = 0;
			else
			state = 128;

		}
}
//LED Manager for multiple LEDs brightness command
// Used to parse which LED group is being effected and update the groups brightness level
void Command_LEDManager_Brightness()
{
	byte _LEDChoice = USBSerial.parseInt(PacketPosition1);
	byte _boardAdd = USBSerial.parseInt(PacketPosition2);
	byte _brightness = USBSerial.parseInt(PacketPosition3);
	byte _enable = USBSerial.parseInt(4);
	
	
	Serial.print(F("LED Single Change, Selected pin;")); Serial.println(_LEDChoice);
	Serial.print(F("LED Single Change, Selected board;")); Serial.println(_boardAdd);
	Serial.print(F("LED Single Change, brightness ;")); Serial.println(_brightness);
	Serial.print(F(", enable;")); Serial.println(_enable? "On" : "Off");

	switch(_boardAdd)
	{
		case 0: Command_RGBOutput_state(); break;
		case 1: if(_enable)_LEDDriver1.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver1.setPWM(_LEDChoice, 0, 4095);break;
		case 2: if(_enable)_LEDDriver2.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver2.setPWM(_LEDChoice, 0, 4095);break;
		case 3: if(_enable)_LEDDriver3.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver3.setPWM(_LEDChoice, 0, 4095);break;
		case 4: if(_enable)_LEDDriver4.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver4.setPWM(_LEDChoice, 0, 4095);break;
		case 5: if(_enable)_LEDDriver5.setPWM(_LEDChoice, _brightness*4, 4095);else _LEDDriver5.setPWM(_LEDChoice, 0, 4095);break;
	}
	
	
}

//LDR block state manager
// Used to control the enable state of the LDR blocks which control the local LED states
void Command_LDRBlockManager_State()
{
	byte LDRBlockChoice = (byte)USBSerial.parseInt(PacketPosition1);
	boolean newState = USBSerial.parseInt(PacketPosition2);
	
	Serial.print(F("LDR Block state change, LDR selected;")); Serial.print(LDRBlockChoice);
	Serial.print(F(", New state;")); Serial.println(newState? "On" : "Off");
	
	switch (LDRBlockChoice)
	{
		case 0: AutoManager_LDR_Block1.setEnable(newState); AutoManager_LDR_Block1.refresh(); break;
	}
}

//Servo Position Handler
// Used to control the position of servo states on the track
void Command_ServoPosition()
{
	byte ServoBlockChoise = (byte)USBSerial.parseInt(PacketPosition1);
	boolean newState = (boolean)USBSerial.parseInt(PacketPosition2);
	
	Serial.print(F("Servo block state change, Servos selected;")); Serial.print(ServoBlockChoise);
	Serial.print(F(", New state;")); Serial.println(newState? "Open":"Close");
	
	switch (ServoBlockChoise)
	{
		//case 0: _Servo1.setPosition(newState); break;
	}
}