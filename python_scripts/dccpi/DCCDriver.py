import dcc_rpi_encoder
from dcc_rpi_encoder import *
from dcc_controller import *
from dcc_locomotive import *

test = False

e = DCCRPiEncoder()
controller = DCCController(e)  # Create the DCC controller with the RPi encoder
l1 = DCCLocomotive("DCC6", 6)  # Create locos, args: Name, DCC Address (see DCCLocomotive class)
l2 = DCCLocomotive("DCC7", 7)
l3 = DCCLocomotive("DCC8", 8)
controller.register(l1)        # Register locos on the controller
controller.register(l2)
controller.register(l3)
controller.start()             # Start the controller. Removes brake signal
if test is True:
	controller.start()             # Start the controller. Removes brake signal
	l1.reverse()                   # Change direction bit
	l2.fl = True                   # Change fl function bit
	l3.fl = True
	l1.speed = 10                  # Change speed
	l2.speed = 18
	l3.speed = 23
	l3.slower()                    # Reduce 1 speed step
	l3.faster()                    # Increase 1 speed step
	
print (l1)                             # Print loco information