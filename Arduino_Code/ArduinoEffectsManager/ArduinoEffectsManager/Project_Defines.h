// Project_Defines.h

#ifndef _PROJECT_DEFINES_h
#define _PROJECT_DEFINES_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif




#include "PhotoResistorTrigger.h"
#include "Defines_PinList.h"
#include "LED_Group.h"
#include "RGBManager.h"
#include "PhotoResLEDManager_Group.h"
#include "Comms_SerialExtended.h"
#include "HW_Pin.h"
#include "AdafruitPWMLib/Adafruit_PWMServoDriver.h"

//******************************************************************
//Serial Comms Handler Class
Comms_SerialExtendedClass USBSerial;

//******************************************************************
//Hardware LED Handler
HW_PinClass TeensyLED;

//******************************************************************
//Hardware I2C Servo addresses
#define LEDDriver1_Add 0x40
#define LEDDriver2_Add 0x41
#define LEDDriver3_Add 0x42
#define LEDDriver4_Add 0x43
#define LEDDriver5_Add 0x44
//******************************************************************
//Hardware I2C Servo and LED controllers
Adafruit_PWMServoDriver _LEDDriver1(LEDDriver1_Add);
Adafruit_PWMServoDriver _LEDDriver2(LEDDriver2_Add);
Adafruit_PWMServoDriver _LEDDriver3(LEDDriver3_Add);
Adafruit_PWMServoDriver _LEDDriver4(LEDDriver4_Add);
Adafruit_PWMServoDriver _LEDDriver5(LEDDriver5_Add);

#define MaxDriverChannels 16

//******************************************************************
//LED Group Classes for grouping LED functionality
LED_GroupClass _StreetLights;
int _numbStreetLights = 2;
int _StreetLightAddressList[] = {pinStreet1LED, pinStreet2LED};
int _StreetLightBrightnessList[] = {255,10};
#define StreetLightFadeRate 500


LED_GroupClass _HouseLights;
int _numbHouseLights = 2;
int _HouseLightsAddressList[] = {pinHouseTopLED, pinHouseBotLED};
int _HouseLightsBrightnessList[] = {255,128};
#define HouseLightFadeRate 50
//******************************************************************
//Top Level LED grouping for grouping LED sets together by area on the layout
LED_GroupClass *LEDs_Block1[2] = {&_StreetLights, &_HouseLights}; //group list for block 1 related to LDR

//******************************************************************
//Photo Resistor sensor class
PhotoResistorTriggerClass LDR_Block1; // Photoresistor wrapper class

//******************************************************************
//Photo Resistor & LED Group manager class that handles switching of LED groups based on Photoresistor feedback
PhotoResLEDManager_GroupClass AutoManager_LDR_Block1; // LDR and LED group manager for automation

//******************************************************************
//Class Defines for LED fade rates

#define RGBFadeRate 20


#endif