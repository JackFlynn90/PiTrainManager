import serial
import time
import redis
from DebugPrint import debugging

import re
import subprocess
from subprocess import PIPE, run

from DccSprog import SprogDevice
from DccPacketHandler import PacketBuilder

import os
import sys
sys.path.append('/share/djangoTrain')
os.environ['DJANGO_SETTINGS_MODULE'] = 'mysite.settings'
import django
from django.conf import settings
django.setup()
from  trains.models import Train



byPassSerial = False
	
	

sprogPort = '/dev/ttyACM1'

debugLevel = 2
debug = debugging()

debug.setLevel(debugLevel) #Debug printing setup. Change level to get different message outputs

r = redis.StrictRedis(host='localhost', port=6379)                          # Connect to local Redis instance

p = r.pubsub()                                                              # See https://github.com/andymccurdy/redis-py/#publish--subscribe
p.subscribe('trainCommand')                                           # Subscribe to startScripts channel

sprog = SprogDevice(sprogPort,debugLevel,byPassSerial)

packetMaker = PacketBuilder(debugLevel)

while True:
	try:
		sprog.openPort()		

		
		RUN = True
		newMessage = True
		
		while RUN:
			try:# Will stay in loop until START message received
				if newMessage is True: 
					debug.Print("Waiting For new redis data",2)
					newMessage = False
					
				message = p.get_message()			# Checks for message
				
				if message:
					command = message['data']	# Get data from message
					debug.Print("PUBSUB Received   " + str(message['data']) ,4)
					
					debug.Print("command is",1)
					debug.Print(type(command),1)
					if type(command) is int:
						debug.Print("Sub connected",2)
					else:
						commandList = command.decode("utf-8")
						commandList = commandList.split("_")
						debug.Print("Command list" + str(commandList),3)
						
						debug.Print("command list is",1)
						debug.Print(type(commandList),1)
						
						if commandList[2] == "Speed":
							Traddress = commandList[1]
							speed = commandList[3]
							
							activeTrain = Train.objects.get(address = Traddress)
							debug.Print("Active train address is;" + str(activeTrain.address),3)
							debug.Print("New speed value is;" + str(speed), 3)
							activeTrain.speed = speed
							activeTrain.save()
	
							packet = packetMaker.SpeedDir(activeTrain)

							sprog.WritePacket(packet)
							
							print ("Packet out; " + packet)
						
						if commandList[2] == "Dir":
							Traddress = commandList[1]
							
							if commandList[3] == "FWD":
								dir = True
							elif commandList[3] == "BWD":
								dir = False
								
							activeTrain = Train.objects.get(address = Traddress)
							debug.Print("Active train address is;" + str(activeTrain.address),3)
							debug.Print("New direction value is;" + str(commandList[3]), 3)
							activeTrain.direction = dir
							activeTrain.save()
							
							packet = packetMaker.SpeedDir(activeTrain)

							sprog.WritePacket(packet)
							
							print ("Packet out; " + packet)

				sprog.PrintFeedback()

			except Exception as e:
				print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
				print(str(e))
				RUN = False
			
	except KeyboardInterrupt:
		print("Keyboard Shutdown")
		sprog.shutdown()
		exit()


