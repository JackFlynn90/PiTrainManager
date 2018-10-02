import serial
import time
import redis
import pickle

import re
import subprocess
from subprocess import PIPE, run

while True:
	try:
		while True:
			try:
				ser = serial.Serial('/dev/ttyACM0', 9600, timeout=10)
				print("Serial port opened")
				break
			except:
				print("Waiting for Serial port open")
				time.sleep(5)
				


		state = 1

		time.sleep(.500)

		dataOut = ":2,1,\n"
		print("Sending Out;")
		print(dataOut)
		ser.write(dataOut.encode())

		dataOut = ":1,255,0,0,\n"
		print("Sending Out;")
		print(dataOut)
		ser.write(dataOut.encode())

		r = redis.StrictRedis(host='localhost', port=6379)                          # Connect to local Redis instance

		p = r.pubsub()                                                              # See https://github.com/andymccurdy/redis-py/#publish--subscribe
		p.subscribe('buttonPress')                                           # Subscribe to startScripts channel

		RUN = True
		newMessage = True
		
		while RUN:
			try:# Will stay in loop until START message received
				if newMessage is True: 
					print("Waiting For new redis data")
					newMessage = False
					
				message = p.get_message()			# Checks for message
				
				if message:
					command = message['data']	# Get data from message
					print("PUBSUB Received")
					print (message['data'])
					
					print("command is")
					print(type(command))
					if type(command) is int:
						print("Sub connected")
					else:
						commandList = command.decode("utf-8")
						commandList = commandList.split("_")
						print("Command list" + str(commandList))
						
						print("command list is")
						print(type(commandList))
						
						if commandList[0] == "#":
							print("Hex value received")
							hex = commandList[1]
							r = int(hex[0] + hex[1], 16)
							g = int(hex[2] + hex[3], 16)
							b = int(hex[4] + hex[5], 16)
							
							dataOut = ":1," + str(r) + "," + str(g) + "," + str(b) + ",\n"
							print("Sending Out;")
							print(dataOut)
							ser.write(dataOut.encode())
						
						elif commandList[0] == "Light":
							dataOut = ":" + commandList[1] + "," + commandList[2] + ",\n"
							print("Sending Out;")
							print(dataOut)
							ser.write(dataOut.encode())
					
				while ser.inWaiting():
					feedback=ser.readline()
					print(feedback)
					
				#time.sleep(0.3)
			except Exception as e:
				print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
				print(str(e))
				RUN = False
				ser.close()
			
	except Exception as e:
		print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
		print(str(e))
		exit()

