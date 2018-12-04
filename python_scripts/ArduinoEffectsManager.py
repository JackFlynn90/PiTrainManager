import serial
import time
import redis
import pickle
from DebugPrint import debugging

import re
import subprocess
from subprocess import PIPE, run

debugLevel = 3
debug = debugging()

debug.setLevel(debugLevel)

r = redis.StrictRedis(host='localhost', port=6379)                          # Connect to local Redis instance

p = r.pubsub()                                                              # See https://github.com/andymccurdy/redis-py/#publish--subscribe
p.subscribe('lightCommand')                                           # Subscribe to startScripts channel

while True:
	try:
		while True:
			try:
				ser = serial.Serial('/dev/ttyACM0', 9600, timeout=10)
				debug.Print("Serial port opened",3)
				break
			except:
				debug.Print("Arduino - Waiting for Serial port open",2)
				time.sleep(5)
				


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
						
						if commandList[0] == "#":
							debug.Print("Hex value received",2)
							hex = commandList[1]
							r = int(hex[0] + hex[1], 16)
							g = int(hex[2] + hex[3], 16)
							b = int(hex[4] + hex[5], 16)
							
							dataOut = ":1," + str(r) + "," + str(g) + "," + str(b) + ",\n"
							debug.Print("Sending Out;" + dataOut,4)
							ser.write(dataOut.encode())
						
						elif commandList[0] == "Light":
							dataOut = ":" + commandList[1] + "," + commandList[2] + "," + commandList[3] + ",\n"
							debug.Print("Sending Out;" + dataOut,4)
							ser.write(dataOut.encode())
					
				while ser.inWaiting() > 0:
					feedback=ser.readline()
					debug.Print(feedback,2)
					
				time.sleep(0.01)
			except Exception as e:
				print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
				print(str(e))
				RUN = False
				ser.close()
			
	except Exception as e:
		print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
		print(str(e))
		exit()

	

