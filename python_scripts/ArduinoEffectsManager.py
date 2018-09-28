import serial
import time
import redis
import pickle
ser = serial.Serial('/dev/ttyACM0', 9600)


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
p.subscribe('buttonPress')                                                 # Subscribe to startScripts channel

RUN = True

while RUN:
	try:# Will stay in loop until START message received
		print("Waiting For redis data")
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
				
				if commandList[0] == "Light":
					dataOut = ":" + commandList[1] + "," + commandList[2] + ",\n"
					print("Sending Out;")
					print(dataOut)
					ser.write(dataOut.encode())
			
		while ser.inWaiting():
			feedback=ser.readline()
			print(feedback)
			
		time.sleep(1)
	except Exception as e:
		print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
		print(str(e))
