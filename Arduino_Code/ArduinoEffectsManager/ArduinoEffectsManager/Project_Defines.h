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
#include "ServoManager.h"

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
#define ServoDriver_Add 0x44
//******************************************************************
//Hardware I2C Servo and LED controllers
Adafruit_PWMServoDriver _LEDDriver1 = Adafruit_PWMServoDriver(LEDDriver1_Add);
Adafruit_PWMServoDriver _LEDDriver2 = Adafruit_PWMServoDriver(LEDDriver2_Add);
Adafruit_PWMServoDriver _LEDDriver3 = Adafruit_PWMServoDriver(LEDDriver3_Add);
Adafruit_PWMServoDriver _LEDDriver4 = Adafruit_PWMServoDriver(LEDDriver4_Add);

Adafruit_PWMServoDriver _ServoDriver = Adafruit_PWMServoDriver(ServoDriver_Add);

#define MaxDriverChannels 16

//******************************************************************
//LED Group Classes for grouping LED functionality
LED_GroupClass _StreetLights;
const byte _numbStreetLights = 6;
const byte _StreetLightAddressList[6] = {DR2_STL1, DR2_STL2,DR2_STL3,DR2_STL4,DR2_STL5,DR2_STL6};
byte _StreetLightBrightnessList[_numbStreetLights];
#define StreetLightFadeRate 500
#define StreetLightDriver _LEDDriver2


LED_GroupClass _HouseLights;
const byte _numbHouseLights = 6;
const byte _HouseLightsAddressList[6] = {DR1_Building1T, DR1_Building1B,DR1_Building2T,DR1_Building2B,DR1_Building3T,DR1_Building3B};
byte _HouseLightsBrightnessList[_numbHouseLights];
#define HouseLightFadeRate 50
#define HouseLightDriver _LEDDriver1

//******************************************************************
//Servo Classes for grouping servo functionality
ServoManagerClass _Servo1;
byte _ServoAndLEDAddressList1[3] = {Servo1,Servo1_LEDG,Servo1_LEDR};
byte _ServoAndLEDAddressList2[3] = {Servo2,Servo2_LEDG,Servo2_LEDR};
#define ServoDriver _ServoDriver


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
//Class Defines for RGB LEDs
#define RGBFastLEDPin 17
#define RGBBrightness 128
#define RGBFadeRate 20


#endif