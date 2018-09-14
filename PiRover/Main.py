#!python
#encoding: utf-8

# Project Overview:
# Connectors handle the network connections (naturally), although Bluetooth is really the only one right now,
# Commander is where PyGame and all the fun logic stuff happens on the host (e.g. laptop) end, converting arrow key input into text commands,
# Controller turns the text commands into IO output on the device, i.e. the Pi.
# New setups probably only have to tweak the pins at the top of Controller, everything else can stay the same initially, assuming that all the Bluetooth code still works...
# This script is naturally the main entry point, and normally runs everything in Device (Pi) mode; just specify 'host' as a command-line parameter to switch that.
# All the other files, if executed alone, should start a test mode of their own functionality.

# Once there's other connection types that actually work, preferably make this switchable at runtime...
from RoverConnectorBluetooth import RoverConnectorBluetooth
import sys

max_command_size = 15

def main():
	print("*** Welcome to the Rover control script ***")
	is_host = False
	if(len(sys.argv) > 1):
		if((sys.argv[1].lower() == "true") or (sys.argv[1].lower() == "host") or (sys.argv[1] == "1")):
			is_host = True

	rc = RoverConnectorBluetooth(is_host)
#	try:
	if(is_host):
		from RoverCommander import RoverCommander
		print("*** Running in Host mode")
		if(rc.connect()):
			rcmd = RoverCommander(rc)
			while rcmd.update():
				# Don't hog the CPU
				time.sleep(0.001)
			rc.disconnect()
			return True
		else:
			# Just abort; the internal connector code will have printed a more specific error
			raise Exception
	else:
		from RoverController import RoverController
		print("*** Running in Device mode")
		if(rc.listen()):
			if(rc.accept()):
				rcont = RoverController(rc)
				# Assume this will block until it receives and processes something
				while rcont.update(): continue
				rc.disconnect()
				return True
			else:
				# Just abort; the internal connector code will have printed a more specific error
				raise Exception
#	except:
#		print("*** Failed to connect. Aborting... ***")
#		return False


if __name__ == '__main__':
	main()
