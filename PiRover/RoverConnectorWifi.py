#!python
#encoding: utf-8

import RoverConnector

# Just a placeholder for right now, and that's assuming there's ever
# even a need for a direct-WiFi connection vs. a regular IP socket...
# Could be handy for outdoors though, since WiFi generally seems to
# have a longer maximum radio range than Bluetooth, except maybe Class 1.
class RoverConnectorWifi(RoverConnector.RoverConnector):
	def __init__(self, hostMode):
		pass


	def listen(self):
		return True


	def accept(self, maxTries):
		return True


	def connect(self):
		return True


	def send(self, data):
		print(">>[RoverConnector.send] Dummy data sent: '%s'" % data)


	def recv(self, maxLength):
		return ""


	def setBlocking(self, blocking):
		pass


	def disconnect(self):
		pass


	def runTestMode(self):
		print("*** Welcome to the RoverConnector WiFi test mode ***")
		if(self.is_host):
			print("Running in Host mode")
			if(self.connect()):
				print("Sending data...")
				self.send("Hello WiFi world!")
				time.sleep(1)
				self.send("Hello?")
				time.sleep(1)
				self.send("Anyone?")
				self.disconnect()
		else:
			print("Running in Device mode")
			if(self.listen()):
				if(self.accept(2)):
					while True:
						data = self.recv(1024)
						if len(data) == 0: break
						print("Received message: '%s'" % data)
					self.disconnect()


if __name__ == '__main__':
	hostMode = True
	if(len(sys.argv) > 1):
		if((sys.argv[1].lower() == "false") or (sys.argv[1].lower() == "device") or (sys.argv[1] == "0")):
			hostMode = False
	rconn = RoverConnectorWifi(hostMode)
	rconn.runTestMode()
