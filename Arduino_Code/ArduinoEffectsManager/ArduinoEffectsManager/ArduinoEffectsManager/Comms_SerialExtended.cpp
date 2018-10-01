//
//
//

#include "Comms_SerialExtended.h"

void Comms_SerialExtendedClass::init()
{


}

// Serial Setup on specific hardware port. No Defines for Layouts or Timeout. Use default timeout of 100ms
void Comms_SerialExtendedClass::setup(HardwareSerial *SerialPort, int BaudRate)
{
	_SerialPort = SerialPort;
	_isSerialUSB = false;
	_SerialPort->begin(BaudRate);
	_Timeout = 100;
	_PacketLayout = DefaultPacketLayout;
	_index = 0;
}

// Serial Setup on specific hardware port. Setup all class variables to match inputs
void Comms_SerialExtendedClass::setup(HardwareSerial *SerialPort, int BaudRate, PacketLayout Layouts, unsigned long TimeOut)
{
	_SerialPort = SerialPort;
	_isSerialUSB = false;
	_SerialPort->begin(BaudRate);
	_Timeout = TimeOut;
	_PacketLayout = Layouts;
	_index = 0;
}

//Setup on USB Serial port. No Defines for Layouts or Timeout. Use default timeout of 100ms
void Comms_SerialExtendedClass::setup()
{

	_isSerialUSB = true;
	_SerialUSB.begin(115200);
	_Timeout = 100;
	_PacketLayout = DefaultPacketLayout;
	_index = 0;
}

// Setup on USB Serial port. Setup all class variables to match inputs
void Comms_SerialExtendedClass::setup(PacketLayout Layouts, unsigned long TimeOut)
{
	_isSerialUSB = true;
	_SerialUSB.begin(115200);
	_Timeout = TimeOut;
	_PacketLayout = Layouts;
	_index = 0;
}

boolean Comms_SerialExtendedClass::Read()
{

	boolean isNewData = false;

	if(_isSerialUSB)
	isNewData = ReadUSB();
	else
	isNewData = ReadSerial();


	return isNewData;
}

//**************************************************************************************************
boolean Comms_SerialExtendedClass::ReadUSB()
{
	boolean isNewData = false;
	
	boolean isValid = false; // start and end characters detected in correct order
	
	unsigned long timeouttimer = millis();
	
	// Check if any data in port. Return if no data waiting;
	if(Serial.available() > 0)
	{
		if(_PrintIncomming)
		Serial.print("+,Echo ;");
		// Timeout while loop - gives data time to arrive in buffer
		while(millis() - timeouttimer < _Timeout || Serial.available())
		{
			// Only act on data if data is in the availalbe buffer
			if(Serial.available() > 0)
			{
				// Prevent going out of bounds of receivedData array size
				if (_index <= StringBufferSize)
				{
					// Pull char from buffer
					char aChar = NULL;
					
					if(_isSerialUSB)
					aChar = Serial.read();
					else
					aChar = _SerialPort->read();

					
					if(_PrintIncomming && aChar != ':')
					Serial.print(aChar); //Print debug
					
					// Start character valid
					if ((aChar == _PacketLayout.StartChar  || _PacketLayout.StartChar == NULL) && _index == 0)
					_StartCharValid = true;
					
					// Start character found mid packet - scraps current packet and reads from new start
					if(aChar == _PacketLayout.StartChar && _StartCharValid == true && _index != 0 && _PacketLayout.StartChar != NULL)
					{
						_index = 0;
						_SeperatorCount = 0;
					}
					
					// End character valid - implies complete packet
					if ((aChar == _PacketLayout.EndChar) && _StartCharValid == true)
					isValid = true;
					
					
					// Seperator Counter to find number of expected values
					if(aChar == _PacketLayout.Seperator && _StartCharValid == true && _index != 0 && _PacketLayout.Seperator != NULL)
					_SeperatorCount++;
					
					// if final char and packet is valid then CRC check and return
					if(aChar == _PacketLayout.EndChar && isValid)
					{

						_CurrentPacket[_index] = NULL; // end final char as NULL - good practice for Char Arrays
						
						//CRCCheck
						// 						if(UseCRC)
						// 						isComplete = CRC_ReceivedPacket(receivedData, index);
						// 						else
						// 						isComplete = isValid;
						
						_FinalCharIndex = _index;
						_FinalSeperatorCount = _SeperatorCount;
						
						_SeperatorCount = 0;
						
						strcpy(_LastValidPacket, _CurrentPacket);
						
						_index = 0;
						isNewData = true;
						_StartCharValid = false;
						
						return isNewData;
					}else
					{
						// Store incomming char data
						_CurrentPacket[_index] = aChar;
						
						if(_StartCharValid && aChar != 0xFF) // prevents data storage of false starts
						_index++;
						
						_CurrentPacket[_index] = '\0'; // Keep the string NULL terminated
					}
					
				}
				else
				{
					// Error Case:
					
					// clear buffer and report issue with data read - prevents flooding of serial causing crashes
					Serial.print("@,Serial USB Instruction too long. Index over flow. Invalid Input");
					//Serial_Clear_USB();
					isValid = false;
					_StartCharValid = false; // start character detected
					
					for (int i = 0; i < StringBufferSize; i++)
					_CurrentPacket[i] = NULL;
					
					return isNewData;
				}
			}
		}

		if(_PrintIncomming)
		{
			Serial.print("+,Serial USB Data timeout. Incomplete Packet Read;\n");
			Serial.print("+,Echo;");
			
			for (int i = 1; i < _index && i < StringBufferSize; i++)
			Serial.print(_CurrentPacket[i]);
			
			Serial.print("\n");
		}
		
	}
	return isNewData;
}//**************************************************************************************************
boolean Comms_SerialExtendedClass::ReadSerial()
{
	boolean isNewData = false;
	
	boolean isValid = false; // start and end characters detected in correct order
	
	unsigned long timeouttimer = millis();
	
	// Check if any data in port. Return if no data waiting;
	if(_SerialPort->available() > 0)
	{
		if(_PrintIncomming)
		Serial.print("+,Echo ;");
		// Timeout while loop - gives data time to arrive in buffer
		while(millis() - timeouttimer < _Timeout || _SerialPort->available())
		{
			// Only act on data if data is in the availalbe buffer
			if(_SerialPort->available() > 0)
			{
				// Prevent going out of bounds of receivedData array size
				if (_index <= StringBufferSize)
				{
					_recordedIndex = _index;
					// Pull char from buffer
					char aChar = NULL;

					aChar = _SerialPort->read();

					
					if(_PrintIncomming && aChar != ':')
					Serial.print(aChar); //Print debug
					
					// Start character valid
					if ((aChar == _PacketLayout.StartChar  || _PacketLayout.StartChar == NULL) && _index == 0)
					_StartCharValid = true;
					
					// Start character found mid packet - scraps current packet and reads from new start
					if(aChar == _PacketLayout.StartChar && _StartCharValid == true && _index != 0 && _PacketLayout.StartChar != NULL)
					{
						_index = 0;
						_SeperatorCount = 0;
					}
					
					// End character valid - implies complete packet
					if ((aChar == _PacketLayout.EndChar) && _StartCharValid == true)
					isValid = true;
					
					
					// Seperator Counter to find number of expected values
					if(aChar == _PacketLayout.Seperator && _StartCharValid == true && _index != 0 && _PacketLayout.Seperator != NULL)
					_SeperatorCount++;
					
					// if final char and packet is valid then CRC check and return
					if(aChar == _PacketLayout.EndChar && isValid)
					{

						_CurrentPacket[_index] = NULL; // end final char as NULL - good practice for Char Arrays
						
						//CRCCheck
						// 						if(UseCRC)
						// 						isComplete = CRC_ReceivedPacket(receivedData, index);
						// 						else
						// 						isComplete = isValid;
						
						_FinalCharIndex = _index;
						_FinalSeperatorCount = _SeperatorCount;
						
						_SeperatorCount = 0;
						
						strcpy(_LastValidPacket, _CurrentPacket);
						
						_index = 0;
						isNewData = true;
						_StartCharValid = false;
						
						return isNewData;
					}else
					{
						// Store incomming char data
						_CurrentPacket[_index] = aChar;
						
						if(_StartCharValid && aChar != 0xFF) // prevents data storage of false starts
						_index++;
						
						_CurrentPacket[_index] = '\0'; // Keep the string NULL terminated
					}
					
				}
				else
				{
					// Error Case:
					
					// clear buffer and report issue with data read - prevents flooding of serial causing crashes
					Serial.print("@,Serial Port Read Instruction too long. Index over flow. Invalid Input");
					//Serial_Clear_USB();
					isValid = false;
					_StartCharValid = false; // start character detected
					
					for (int i = 0; i < StringBufferSize; i++)
					_CurrentPacket[i] = NULL;
					
					return isNewData;
				}
			}
		}

		if(_PrintIncomming)
		{
			Serial.print("+,Serial Port Data timeout. Incomplete Packet Read;\n");
			Serial.print("+,Echo;");
			
			for (int i = 1; i < _index && i < StringBufferSize; i++)
			Serial.print(_CurrentPacket[i]);
			
			Serial.print("\n");
		}
		
	}
	return isNewData;
}
//**************************************************************************************************

// Parse Data Functions

// Parse single Int from Array Location in the latest packet
int Comms_SerialExtendedClass::parseInt(int ArrayLocation)
{
	int returnVal = 0;
	
	if(ArrayLocation <= _FinalSeperatorCount)
	{
		char tempCpy[StringBufferSize];
		char delimiters[] = {_PacketLayout.StartChar, _PacketLayout.Seperator, _PacketLayout.EndChar, NULL};
		char* CharData;
		
		
		strcpy(tempCpy, _LastValidPacket);
		
		CharData = strtok(tempCpy, delimiters);
		
		for (int i = 0; i <= ArrayLocation; i++)
		{
			returnVal = atoi(CharData);
			CharData = strtok(NULL, delimiters);
		}
		
		return returnVal;
		
	}else
	Serial.print("~,Comms Extended Serial Error - Parsing data out of bounds\n");
	
	return -999;
}

// Parse single Int from Array Location in the latest packet
unsigned long Comms_SerialExtendedClass::parseUL(int ArrayLocation)
{
	int returnVal = 0;
	
	if(ArrayLocation <= _FinalSeperatorCount)
	{
		char tempCpy[StringBufferSize];
		char delimiters[] = {_PacketLayout.StartChar, _PacketLayout.Seperator, _PacketLayout.EndChar, NULL};
		char* CharData;
		
		
		strcpy(tempCpy, _LastValidPacket);
		
		CharData = strtok(tempCpy, delimiters);
		
		for (int i = 0; i <= ArrayLocation; i++)
		{
			returnVal = strtoul(CharData,NULL,10);
			CharData = strtok(NULL, delimiters);
		}
		
		return returnVal;
		
	}else
	Serial.print("~,Comms Extended Serial Error - Parsing data out of bounds\n");
	
	return -999;
}


// Parse single float from Array Location in the latest packet
float Comms_SerialExtendedClass::parseFloat(int ArrayLocation)
{
	float returnVal = 0;
	
	if(ArrayLocation <= _FinalSeperatorCount)
	{
		char tempCpy[StringBufferSize];
		char delimiters[] = {_PacketLayout.StartChar, _PacketLayout.Seperator, _PacketLayout.EndChar, NULL};
		char* CharData;
		
		
		strcpy(tempCpy, _LastValidPacket);
		
		CharData = strtok(tempCpy, delimiters);
		
		for (int i = 0; i <= ArrayLocation; i++)
		{
			returnVal = atof(CharData);
			CharData = strtok(NULL, delimiters);
		}
		
		return returnVal;
		
	}else
	Serial.print("~,Comms Extended Serial Error - Parsing data out of bounds\n");
	
	return -255;
}


// Parse single Int from first location. Generally "command" data
int Comms_SerialExtendedClass::parseCommand()
{
	int returnVal = 0;
	

	char tempCpy[StringBufferSize];
	char delimiters[] = {_PacketLayout.StartChar, _PacketLayout.Seperator, _PacketLayout.EndChar, NULL};
	char* CharData;
	
	
	strcpy(tempCpy, _LastValidPacket);
	
	CharData = strtok(tempCpy, delimiters);
	
	for (int i = 0; i < 1; i++)
	{
		returnVal = atoi(CharData);
		CharData = strtok(NULL, delimiters);
	}
	
	return returnVal;
	

	
	return -255;
}

void Comms_SerialExtendedClass::printLastPacket()
{
				for (int i = 1; i < _FinalCharIndex && i < StringBufferSize; i++)
				Serial.print(_LastValidPacket[i]);
				
				Serial.print("\n");
}

//**************************************************************************************************

//******************************************************************************
//******************************************************************************
// Flush the selected serial port to ensure no remaining data in the buffer
void Comms_SerialExtendedClass::Clear_USB()
{
	while(Serial.available())
	byte  w = Serial.read();
}

//******************************************************************************
//******************************************************************************
// Flush the selected serial port to ensure no remaining data in the buffer
void Comms_SerialExtendedClass::Clear_USBForDelay(unsigned long delayTime)
{
	unsigned long startTime = millis();
	if(_PrintIncomming)
	{
		Serial.print("+,Clearing Serial USB Buffer for Delay;"); Serial.println(delayTime);
	}
	
	while(millis() - startTime < delayTime)
	{
		if(Serial.available())
		byte  w = Serial.read();
	}
}


//**************************************************************************************************//**************************************************************************************************
//**************************************************************************************************//**************************************************************************************************


