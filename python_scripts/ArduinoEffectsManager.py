import serial
import time
import redis
import pickle
from DebugPrint import debugging

import re
import subprocess, traceback
from subprocess import PIPE, run

#Imported django parts to allow updates to database
import os
import sys
sys.path.append('/share/djangoTrain')
os.environ['DJANGO_SETTINGS_MODULE'] = 'mysite.settings'
import django
from django.conf import settings
django.setup()
from  trains.models import Light, Servo, LightGroup

debugLevel = 2 #Debug print class used to handle what level of debug statements we want
debug = debugging()
logFile = '/share/djangoTrain/logs/AEM.log'
debug.setup(debugLevel,logFile)

r = redis.StrictRedis(host='localhost', port=6379)                          # Connect to local Redis instance

p = r.pubsub()                                                              # See https://github.com/andymccurdy/redis-py/#publish--subscribe
p.subscribe('EffectsCommand')                                           # Subscribe to Effects channel

Serialbypass = False #Debug serial port bypass. Sends all serial data out as a debug print instead

ser = serial #global serial port

#Send serial allows for serial data output or bypass depending on debug flag
def SendSerial(Data):
	if Serialbypass is True:
		debug.Print("Serialbypass Out:" + Data,3)
	else:
		ser.write(Data.encode())

#Read serial allows for serial data feedback or bypass depending on debug flag
def ReadSerial():
	if Serialbypass is True:
		debug.Print("Serialbypass waiting for data", 1)
	else:
		while ser.inWaiting() > 0:
			feedback=ser.readline()
			debug.Print(str(feedback),3)

#Main infinite loop
while True:
	try:
		#Open serial port or keep retrying
		while True:
			try:
				ser = serial.Serial('/dev/ttyTeensy', 9600, timeout=10)
				debug.Print("Serial port opened",3)
				break
			except:
				debug.Print("Arduino - Waiting for Serial port open",2)

				if Serialbypass is True: #Bypass usage of serial port
					debug.Print("Serial Bypass Enabled. Ignoring Serial Port",4)
					break
				time.sleep(5)

		RUN = True
		newMessage = True

		#Main loop. Waits for Redis messages and parses and outputs via serial
		while RUN:
			try:# Will stay in loop unless device unplugged

				if newMessage is True: #Waiting for redis messages
					debug.Print("Waiting For new redis data",2)
					newMessage = False


				message = p.get_message()			# Checks for message

				if message:
					command = message['data']	# Get data from message

					#Early debugging code
					debug.Print("PUBSUB Received   " + str(message['data']) ,4)
					debug.Print("command is",1)
					debug.Print(str(type(command)),1)

					#Detect if message is reply to redis subscription
					if type(command) is int:
						debug.Print("Sub connected",2)
					else:# Handle command received

						#Decode redis command message
						commandList = command.decode("utf-8")
						commandList = commandList.split("_")

						#Early debugging code
						debug.Print("Command list" + str(commandList),3)
						debug.Print("command list is",1)
						debug.Print(str(type(commandList)),1)

						#RGB colour message
						if commandList[0] == "#":
							debug.Print("Hex value received",2)
							hex = commandList[1]
							r = int(hex[0] + hex[1], 16)
							g = int(hex[2] + hex[3], 16)
							b = int(hex[4] + hex[5], 16)

							#Construct Arduino packet
							dataOut = ":1," + str(r) + "," + str(g) + "," + str(b) + ",\n"

							debug.Print("Sending Out;" + dataOut,4)
							SendSerial(dataOut)

							activeLight = Light.objects.get(type = "RGB") #database object
							activeLight.hexValue = hex
							activeLight.save()

						#LED Light group command
						elif commandList[0] == "Light":
							Lightaddress = commandList[2] # get light address from packet
							activeLight = Light.objects.get(pk = Lightaddress) #database object

							if commandList[1] == "4": #On/Off state change database value
								if commandList[6] == "1":
									activeLight.lightsState = True
								else:
									activeLight.lightsState = False
							elif commandList[1] == "5": #Brigthness change database value
								activeLight.brightness = commandList[5]

							lightbright = str(activeLight.brightness)
							lightstate = str(int(activeLight.lightsState))

							dataOut = ":" + commandList[1] + "," + commandList[3] + "," + commandList[4] + "," + lightbright + "," + lightstate + ",\n" #construct packet


							debug.Print("Sending Out;" + dataOut,4)
							SendSerial(dataOut) #Send out data to arduino

							activeLight.save()# Save database changes

							#LED Light group command
						elif commandList[0] == "lightgroup":
								#Packet example - lightgroup_4_{{lightgroup.pk}}_0
								LightGroupaddress = commandList[2] # get light address from packet
								activeGroup = LightGroup.objects.get(pk = LightGroupaddress) #database object

								commandType = commandList[1] # either 4 or 5 for on/off command or brigthness

								if commandType == "4":
									debug.Print("Light Group, command is on/off", 2)

									if commandList[3] == "1": #change group state to match command value
										activeGroup.lightsState = True
									else:
										activeGroup.lightsState = False
								else:
									debug.Print("Light Group, command is brightness", 2)
									activeGroup.brightness = commandList[3] #brightness value

								activeGroup.save() #save all changes to database

								groupState = str(int(activeGroup.lightsState))
								groupBrightness = str(int(activeGroup.brightness))

								debug.Print("Light Group, State is;" + groupState, 1)
								debug.Print("Light Group, Brigthness is;" + groupBrightness, 1)

								#get all lights in the grouping
								debug.Print("Lights in Group;", 5)
								lightsinGroup = Light.objects.filter(lightgroup__pk=LightGroupaddress)
								debug.Print(str(lightsinGroup), 5)

								#cycle through lights in group and send out data to teensy
								for lights in lightsinGroup:
									debug.Print(str(lights.title), 2)

									newLightState = str(groupState)
									newLightBrighness = str(groupBrightness)

									lightBoard = str(lights.boardAddress)
									lightAddress = str(lights.address)

									lights.lightsState = newLightState
									lights.brightness = newLightBrighness


															#command, pk, led number, board number, brightness, enable
									dataOut = ":" + commandType + "," + lightAddress + "," + lightBoard + "," + newLightBrighness + "," + newLightState + ",\n" #construct packet

									debug.Print("Sending Out for " + str(lights.title) + ";" + dataOut,4)

									SendSerial(dataOut) #Send out data to arduino

									lights.save()# Save database changes

						#Servo positional on/off command
						elif commandList[0] == "Servo":

							Servopk = commandList[2] # get servo address from packet
							activeServo = Servo.objects.get(pk = Servopk) #get related database object

							if commandList[3] == "1":
								activeServo.state = True
							else:
								activeServo.state = False

							activeServo.save()

							ServoBoard = str(int(activeServo.boardAddress))
							ServoAddress = str(int(activeServo.address))
							ServoPosThrown = str(int(activeServo.throwPosition))
							ServoState = str(int(activeServo.state))

							dataOut = ":" + commandType + "," + ServoAddress + "," + ServoBoard + "," + ServoPosThrown + "," + ServoState + ",\n" #construct packet

							debug.Print("Sending Out;" + dataOut,4)
							SendSerial(dataOut) #Send out serial command

							activeServo.save() 	#Update database changes

				ReadSerial() #readback any serial data from the Arduino

				time.sleep(0.1)

			#Exception may occur on loss of connection to Arduino
			except Exception as e:
				debug.Print("!!!!!!!!!! EXCEPTION !!!!!!!!!", 4)
				debug.Print(traceback.format_exc(),4)
				debug.Print(str(e),4)
				RUN = False
				ser.close()

	except Exception as e:
		debug.Print("!!!!!!!!!! EXCEPTION !!!!!!!!!",4)
		debug.Print(str(e),4)
		exit()
