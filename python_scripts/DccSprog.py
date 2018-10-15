import serial
import time
from DebugPrint import *

#Sprog DCC driver class.
# Handler for the serial port to the Sprog
class SprogDevice():
	
	ser = serial
	debug = debugging()
	byPassSerial = False
	
	#Class setup. 
	#Requires port for the sprog, debug print level for printing outputs and allows for bypassing the serial port during testing
	def __init__(self, serialPort, debuglevel, bypassSerial):
		self.sprogPort = serialPort

		debugLevel = debuglevel
		self.debug = debugging()

		self.debug.setLevel(debugLevel) #Debug printing setup. Change level to get different message outputs
		self.byPassSerial = bypassSerial
		
	#Packet output to the Sprog via serial port
	def WritePacket(self,packet):
		#Serial port bypass for debugging without sprog connected
		if self.byPassSerial is True:
			self.debug.Print("Packet write;" + packet, 5)
		else:
			try:
				self.ser.write(packet.encode())
			except: #exception catch when serial port is disconnected
				print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
				print(str(e))
				return False
	
	
	#Print out of feedback data from the Sprog via serial
	def PrintFeedback(self):
		if self.byPassSerial is True:
			return
		else:
			try:				
				while self.ser.inWaiting() > 0:
					feedback= self.ser.read()
					self.debug.Print(feedback,2)
					time.sleep(0.01)
						
			except Exception as e:
				print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
				print(str(e))
				RUN = False
				self.ser.close()
				return False
		
	#Open port at startup. Blocking until port opens unless it's debugging bypass
	def openPort(self):
		while True:
			try:
				if self.byPassSerial is True:
					break
				
				self.ser = serial.Serial(self.sprogPort, 9600, timeout=10)
				self.debug.Print("Serial port opened",3)
				
				pack1 = "+ \r\n" #packet turns on the DCC power to the rails
				time.sleep(0.1)
				self.ser.write(pack1.encode())
				self.ser.write(pack1.encode())
				self.ser.write(pack1.encode())
				break
			except:
				self.debug.Print("Waiting for Serial port open",4)
				time.sleep(5)
			
			return True
			
	def shutdown(self):
		pack1 = "- \r\n" #packet turns off the DCC power to the rails
		self.ser.write(pack1.encode())
		self.ser.write(pack1.encode())
		self.ser.write(pack1.encode())
		self.ser.close()
		

		


