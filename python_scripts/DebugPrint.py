import logging, logging.handlers
from datetime import datetime

class debugging():
	my_logger = logging.getLogger('GyroReceiveLogger')
	_priorityLevel = 0
	_fileName = "./default.log"
	handler = logging.handlers

	def setup(self, priorityLevel, filename):
		self._priorityLevel = priorityLevel
		self._fileName = filename
		self.SetupLoggers()


	def Print(self, message, priority):
		if int(priority) >= int(self._priorityLevel):
			print(message)
			self.log(message, False)


	##***********************************************************************************************************************
	### Logging helper methods for setup and sending of logging messages
	def log(self,Message, DEBUG=False):
		"""Saves message and timestamp to logfile"""
		Message = datetime.utcnow().strftime('%Y-%m-%d %H:%M:%S:%f')[:-3] + "," + Message
		self.my_logger.debug(Message)
		if DEBUG:
		    print(Message)

	###Setup of loggers to output raw NMEA data to seperate log file
	def SetupLoggers(self):
		self.my_logger = logging.getLogger('default')
		self.my_logger.setLevel(logging.DEBUG)
		LogFile = self._fileName
		self.handler = logging.handlers.TimedRotatingFileHandler(LogFile, when="d", interval=1, backupCount=2)  # Add the log message handler to the logger
		self.my_logger.addHandler(self.handler)
