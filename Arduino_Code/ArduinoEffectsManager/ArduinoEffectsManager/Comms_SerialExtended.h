// Comms_SerialExtended.h

#ifndef _Comms_SerialExtended_h
#define _Comms_SerialExtended_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#define StringBufferSize 64
#define _SerialUSB Serial

#define PacketPosition1 1
#define PacketPosition2 2
#define PacketPosition3 3

//#define _PrintIncomming true;
//#define useSerial

struct PacketLayout
{
	char StartChar;
	char EndChar;
	char Seperator;
};

class Comms_SerialExtendedClass
{
	protected:
	HardwareSerial *_SerialPort;
	char _CurrentPacket[StringBufferSize];
	int _index;
	boolean _StartCharValid;
	boolean _isSerialUSB;
	int _SeperatorCount;
	PacketLayout DefaultPacketLayout = {NULL,'\n',','};
	int _recordedIndex = 0;

	boolean ReadUSB();
	boolean ReadSerial();
	public:
	PacketLayout _PacketLayout;
	int _FinalSeperatorCount;
	int _FinalCharIndex;
	
	
	
	unsigned long _Timeout;
	char _LastValidPacket[StringBufferSize];
	
	void init();
	void setup(HardwareSerial *SerialPort, int BaudRate);
	void setup(HardwareSerial *SerialPort, int BaudRate, PacketLayout Layouts, unsigned long TimeOut);
	void setup();
	void setup(PacketLayout Layouts, unsigned long TimeOut);
	
	
	int parseInt(int ArrayLocation);
	int parseCommand();
	float parseFloat(int ArrayLocation);
	unsigned long parseUL(int ArrayLocation);
	
	boolean Read();

	void printLastPacket();//Print the last stored packet data
	
	void Clear_USB();
	// Flush the selected serial port to ensure no remaining data in the buffer
	void Clear_USBForDelay(unsigned long delayTime);
};


#endif

