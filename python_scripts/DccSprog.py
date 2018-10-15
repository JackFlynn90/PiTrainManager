import serial
import time
from DebugPrint import *


class SprogDevice():
	
	ser = serial
	debug = debugging()
	byPassSerial = False
	
	def __init__(self, serialPort, debuglevel, bypassSerial):
		self.sprogPort = serialPort

		debugLevel = debuglevel
		self.debug = debugging()

		self.debug.setLevel(debugLevel) #Debug printing setup. Change level to get different message outputs
		self.byPassSerial = bypassSerial
		
	def WritePacket(self,packet):
		if self.byPassSerial is True:
			self.debug.Print("Packet write;" + packet, 5)
		else:
			try:
				self.ser.write(packet.encode())
			except:
				print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
				print(str(e))
				return False
		
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
		
	def openPort(self):
		while True:
			try:
				if self.byPassSerial is True:
					break
				
				self.ser = serial.Serial(self.sprogPort, 9600, timeout=10)
				self.debug.Print("Serial port opened",3)
				pack1 = "+ \r\n"
				time.sleep(0.1)
				self.ser.write(pack1.encode())
				self.ser.write(pack1.encode())
				self.ser.write(pack1.encode())
				break
			except:
				self.debug.Print("Waiting for Serial port open",4)
				time.sleep(5)
			
			return True

		


