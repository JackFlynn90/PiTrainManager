class debugging():
	
	_priorityLevel = 0
	
	def setLevel(self, priorityLevel):
		self._priorityLevel = priorityLevel

	def Print(self, message, priority):
		if int(priority) >= int(self._priorityLevel):
			print(message)
	

