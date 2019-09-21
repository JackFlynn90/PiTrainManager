import serial
import time, traceback
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

		logFile = '/share/djangoTrain/logs/DCCSprog.log'
		self.debug.setup(debugLevel,logFile)

		self.byPassSerial = bypassSerial

	#Packet output to the Sprog via serial port
	def WritePacket(self,packet):
		#Serial port bypass for debugging without sprog connected
		if self.byPassSerial is True:
			self.debug.Print("Packet write;" + packet, 5)
		else:
			try:
				self.ser.write(packet.encode())
				return True
			except: #exception catch when serial port is disconnected
				print("!!!!!!!!!! EXCEPTION !!!!!!!!!")
				print(str(e))
				self.ser.close()
				return False


	#Print out of feedback data from the Sprog via serial
	def PrintFeedback(self):
		if self.byPassSerial is True:
			return
		else:
			try:
				while self.ser.inWaiting() > 0:
					feedback= self.ser.read()
					self.debug.Print(str(feedback),2)
					time.sleep(0.01)

				return True
			except Exception as e:
				self.debug.Print("!!!!!!!!!! EXCEPTION !!!!!!!!!", 4)
				self.debug.Print(traceback.format_exc(),4)
				self.debug.Print(str(e),4)
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
				return True
			except:
				self.debug.Print("Sprog - Waiting for Serial port open",5)
				time.sleep(5)


	def close(self):
		self.ser.close()

	def shutdown(self):
		pack1 = "- \r\n" #packet turns off the DCC power to the rails
		self.ser.write(pack1.encode())
		self.ser.write(pack1.encode())
		self.ser.write(pack1.encode())
		self.ser.close()
