import time
from DebugPrint import debugging

import os
import sys
sys.path.append('/share/djangoTrain')
os.environ['DJANGO_SETTINGS_MODULE'] = 'mysite.settings'
import django
from django.conf import settings
django.setup()
from  trains.models import Train

class PacketBuilder():

	debug = debugging()



	def __init__(self, debuglevel):
		self.debug = debugging()
		logFile = '/share/djangoTrain/logs/DccPacketHandler.log'
		self.debug.setup(2,logFile)


	#Speed command packet builder
	def SpeedDir(self,activeTrain):

		#Pull details from train object
		address = activeTrain.address
		speed = activeTrain.speed

		#Build address part of the packet
		if address < 10 :
			strAddress = "0" + str(address)
		else:
			strAddress = str(address)

		#Build speed part of the packet. Requires a couple of checks
		if speed is "1": #A "1" value is used as emergency stop in DCC terms. Increment the value by 1 to 2 to prevent sudden stop at low speed
			speed = "2"

		if int(speed) == 128:
				speed = int(127)

		if activeTrain.direction is True:
			speedOut = int(speed) + 128
		else:
			speedOut = int(speed)

		#Stop reverse from capping out at 128 and causing a stop. DCC slow stop is 128
		if activeTrain.direction is False:
			if int(speed) == 0:
				speedOut = int(128)


		#Ensure extra 0s included at low speed
		if speedOut > 10:
			speedStr = str(format((speedOut),'x'))
		else:
			speedStr = "0" + str(format((speedOut),'x'))

		#Basic 8 bit checksum xor. only of data sent
		checksum = speedOut ^ address ^ 63

		checksum = format(checksum,'x')

		#Packet format "O" -seperated by spaces- address -- packetexpanded -- speed -- checksum -- carriage return
		packet = "O " + strAddress + " 3F " + speedStr + " " + str(checksum) + "\r\n"

		return packet

	def Functions(self,activeTrain):

		#Pull details from train object
		address = activeTrain.address

		lightstate = activeTrain.lightsOn

		if lightstate is True:
			lightValue = 144
		else:
			lightValue = 128

		#Build address part of the packet
		if address < 10 :
			strAddress = "0" + str(address)
		else:
			strAddress = str(address)

		print("before check sum light is;" + str(lightValue) + " , and address is;" + str(address))

		#Basic 8 bit checksum xor. only of data sent
		checksum = address ^ lightValue

		strLight = str(format((lightValue),'x'))

		checksum = format(checksum,'x')


		#Packet format "O" -seperated by spaces- address -- packetexpanded -- speed -- checksum -- carriage return
		packet = "O " + strAddress + " " + strLight + " " + str(checksum) + "\r\n"

		return packet
