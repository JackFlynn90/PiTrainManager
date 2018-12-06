
void setup()
{
	
	//***************************************************************************************************************
	//Serial Comms setup
	USBSerial.setup(); //Custom USB serial comms class for handling incomming packets
	USBSerial._PacketLayout.StartChar = ':';
	USBSerial._PrintIncomming = true; //Debug echo incomming serial data
	
	
	//***************************************************************************************************************
	//RGB LED Setup
	RGBManager.setup(pinRGB_r,pinRGB_g,pinRGB_b, RGBFadeRate);//RGB LED class for handling RGB single LED colour
	
	
	//***************************************************************************************************************
	//i2c Driver Setup
	_LEDDriver1.begin();
	_LEDDriver1.setPWMFreq(60);
	
	_LEDDriver2.begin();
	_LEDDriver2.setPWMFreq(60);
	
	//***************************************************************************************************************
	//LED Group setups. Grouped LEDs allow for easy control of groupings such as "Street lights" or housing

	_StreetLights.setupi2c(&_LEDDriver1,_StreetLightAddressList, _StreetLightBrightnessList,_numbStreetLights,StreetLightFadeRate);
	
	_HouseLights.setupi2c(&_LEDDriver2,_HouseLightsAddressList,_HouseLightsBrightnessList,_numbHouseLights,HouseLightFadeRate);
	
	
	//***************************************************************************************************************
	//Setup for LDR classes
	LDR_Block1.setup(pin_in_PhotoRes, 700);
	
	//Setup for LED-LDR automation linker manager
	AutoManager_LDR_Block1.setup(LDR_Block1,LEDs_Block1, 2);
	
	//Debug LED setup
	TeensyLED.setup(13);
	TeensyLED.setPin(LOW);
	
	Serial.println("Hello World from ArduinoEffectsManager");
	
}
