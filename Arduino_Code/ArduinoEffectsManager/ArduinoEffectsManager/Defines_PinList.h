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


//**********************************************************************************
//Address Assignment for PWM Drivers

//********************
//Building Lights
#define DR1_Building1T 0
#define DR1_Building1B 1
#define DR1_Building2T 2
#define DR1_Building2B 3
#define DR1_Building3T 4
#define DR1_Building3B 5

//**********************
//Street Lights
#define DR2_STL1 0
#define DR2_STL2 1
#define DR2_STL3 2
#define DR2_STL4 3
#define DR2_STL5 4
#define DR2_STL6 5

//************************************************************
//**********************
//Track Switching
#define Servo1 0
#define Servo1_LEDG 1
#define Servo1_LEDR 2
#define Servo2 3
#define Servo2_LEDG 4
#define Servo2_LEDR 5

//*************************************************************************************
//************************************************************

//*****************************************************************************


#endif

