import serial
import time
import redis
from DebugPrint import debugging

import re
import subprocess
from subprocess import PIPE, run

sprogPort = '/dev/ttyACM1'

debugLevel = 2
debug = debugging()

debug.setLevel(debugLevel)

r = redis.StrictRedis(host='localhost', port=6379)                          # Connect to local Redis instance

p = r.pubsub()                                                              # See https://github.com/andymccurdy/redis-py/#publish--subscribe
p.subscribe('trainCommand')                                           # Subscribe to startScripts channel

speed = 13
direction = 'BWD'
dir_FWD = 'FWD'
dir_BWD = 'BWD'
address = 3

while True:
	try:
		while True:
			try:
				ser = serial.Serial(sprogPort, 9600, timeout=10)
				debug.Print("Serial port opened",3)
				pack1 = "+ \r\n"
				time.sleep(0.1)
				ser.write(pack1.encode())
				ser.write(pack1.encode())
				ser.write(pack1.encode())
				break
			except:
				debug.Print("Waiting for Serial port open",4)
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
						
						if commandList[0] == "Tr":
							speed = int(commandList[3])
							debug.Print("New speed Out;" + str(speed),4)
							#packet layout is "O" "Address" "3F" Speed "checksum"

							if address < 10 :
								strAddress = "0" + str(address)
							else:
								strAddress = str(address)
							
							if direction is 'BWD' and speed is 0:
								speedOut = int(128)
							
							if direction is 'FWD':
								speedOut = int(speed) + 128
							else:
								speedOut = int(speed)
							
							
							if speedOut > 10:
								speedStr = str(format((speedOut),'x'))
							else:
								speedStr = "0" + str(format((speedOut),'x'))
								
							data = speedOut ^ address ^ 63
							
							checksum = format(data,'x')
							
							packet = "O " + strAddress + " 3F " + speedStr + " " + str(checksum) + "\r\n"
							
							ser.write(packet.encode())
							
							print ("Packet out; " + packet)

					
				while ser.inWaiting() > 0:
					feedback=ser.read()
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

	

