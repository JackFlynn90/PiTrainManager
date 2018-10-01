// 
// 
// 

#include "HW_Pin.h"

void HW_PinClass::init()
{


}

void HW_PinClass::setup(int PinNum)
{
	_PinNum = PinNum;
	_Status = false;
	
	pinMode(_PinNum, OUTPUT);
	
	digitalWriteFast(_PinNum, _Status);
}

void HW_PinClass::SetPin(int Value)
{
	_Status = Value;
	digitalWriteFast(_PinNum,_Status);
}

void HW_PinClass::toggle()
{
	_Status = !_Status;
	digitalWriteFast(_PinNum,_Status);
}
HW_PinClass HW_Pin;

