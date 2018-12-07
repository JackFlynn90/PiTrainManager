import serial
import time
import redis
import pickle
from DebugPrint import debugging

import re
import subprocess
from subprocess import PIPE, run

debugLevel = 2 #Debug print class used to handle what level of debug statements we want
debug = debugging()

debug.setLevel(debugLevel)

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
			debug.Print(feedback,2)

#Main infinite loop
while True:
	try:
		#Open serial port or keep retrying
		while True:
			try:
				ser = serial.Serial('/dev/ttyACM0', 9600, timeout=10)
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
					debug.Print(type(command),1)
					
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
						debug.Print(type(commandList),1)
						
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
						
						#LED Light group command
						elif commandList[0] == "Light":
							dataOut = ":" + commandList[1] + "," + commandList[2] + "," + commandList[3] + ",\n"
							debug.Print("Sending Out;" + dataOut,4)
							SendSerial(dataOut)
						
						#Servo positional on/off command
						elif commandList[0] == "Servo":
							dataOut = ":" + commandList[1] + "," + commandList[2] + "," + commandList[3] + ",\n"
							debug.Print("Sending Out;" + dataOut,4)
							SendSerial(dataOut)
					
				ReadSerial() #readback any serial data from the Arduino
					
				time.sleep(0.01)
			
			#Exception may occur on loss of connection to Arduino
			except Exception as e:
				print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
				print(str(e))
				RUN = False
				ser.close()
			
	except Exception as e:
		print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
		print(str(e))
		exit()

