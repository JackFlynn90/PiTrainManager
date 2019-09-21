import serial
import time
import redis, traceback
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

debugLevel = 1
debug = debugging()

logFile = '/share/djangoTrain/logs/DCCMan.log'
debug.setup(debugLevel,logFile)

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
					debug.Print(str(type(command)),1)
					if type(command) is int:
						debug.Print("Sub connected",2)
					else:
						commandisValid = False
						#decode command data into a seperated list. seperate by "_" character
						commandList = command.decode("utf-8")
						commandList = commandList.split("_")
						debug.Print("Command list" + str(commandList),3)

						debug.Print("command list is" + str(type(commandList)),1)

						Traddress = commandList[1] # get train address from packet
						activeTrain = Train.objects.get(address = Traddress)

						#Speed command parse data and updates database
						if commandList[2] == "Speed":
							commandisValid = True
							speed = commandList[3]
							debug.Print("Active train address is;" + str(activeTrain.address) + ", New speed value is;" + str(speed),3)
							activeTrain.speed = speed

						#Direction command parse data and updates database
						if commandList[2] == "Dir":
							commandisValid = True
							if commandList[3] == "FWD":
								dir = True
							elif commandList[3] == "BWD":
								dir = False
							debug.Print("Active train address is;" + str(activeTrain.address) + ", New direction value is;" + str(commandList[3]),3)
							activeTrain.direction = dir

						#Train light command parse data and updates database
						if commandList[2] == "Lights":

							#commandisValid = True
							if commandList[3] == "On":
								lightState = True
							elif commandList[3] == "Off":
								lightState = False

							debug.Print("Active train address is;" + str(activeTrain.address) + ", New lights value is;" + str(commandList[3]),3)

							activeTrain.lightsOn = lightState

							packet = packetMaker.Functions(activeTrain)# creates packet for sprog

							RUN = sprog.WritePacket(packet) # Sending packet to sprog

							debug.Print("Packet out; " + packet,3)

						if commandisValid is True:
							#Save changes to db
							activeTrain.save()

							#Send out data to train
							packet = packetMaker.SpeedDir(activeTrain)# creates packet for sprog

							RUN = sprog.WritePacket(packet) # Sending packet to sprog

							debug.Print("Packet out; " + packet,3)
						else:
							debug.Print("Invalid command received;" + str(commandList), 4)

				RUN = sprog.PrintFeedback()

			except Exception as e:
				debug.Print("!!!!!!!!!! EXCEPTION !!!!!!!!!", 4)
				debug.Print(traceback.format_exc(),4)
				debug.Print(str(e),4)
				sprog.close()
				RUN = False

	#Clean exit on keyboard close of script. Shuts down track power
	except KeyboardInterrupt:
		debug.Print("Keyboard Shutdown",4)
		sprog.shutdown()
		exit()

	except Exception as e:
		debug.Print("!!!!!!!!!! EXCEPTION !!!!!!!!!", 4)
		debug.Print(traceback.format_exc(),4)
		debug.Print(str(e),4)
		sprog.shutdown()
		exit()
