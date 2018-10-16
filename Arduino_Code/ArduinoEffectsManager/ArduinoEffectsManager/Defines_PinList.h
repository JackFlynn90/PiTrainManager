// Defines_PinList.h

#ifndef _DEFINES_PINLIST_h
#define _DEFINES_PINLIST_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define pinHouseTopLED 20
#define pinHouseBotLED 3
#define pinStreet1LED 4
#define pinStreet2LED 5


#define pinRGB_r 21
#define pinRGB_g 23
#define pinRGB_b 22

#define pin_in_PhotoRes 14

#endif

