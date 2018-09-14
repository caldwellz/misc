#!python
#encoding: utf-8

# "bluetooth" = latest version of PyBluez
import time, sys, subprocess, bluetooth, RoverConnector

class RoverConnectorBluetooth(RoverConnector.RoverConnector):
	# *** Public functions ***
	def __init__(self, hostMode):
		if(sys.platform.startswith("linux")):
			import dbus

		# Setup-specific variables
		self.host_name = "PC"
		self.device_name = "jammydodge"
		self.service_name = "PiRover"
		self.device_mac = None
		self.print_device_names = True
		self.scan_seconds = 5
		self.scan_tries = 2
		self.comm_channel = bluetooth.PORT_ANY

		# Custom auth is much easier here than trying to figure out PIN-pairing
		# Yeah it's vulnerable to a replay attack but who cares for the moment...
		self.host_auth_string = b"RaspiRocks!"
		self.dev_auth_string = b"YesIDoThankYou"
		self.service_uuid = "b4610e06-65b1-45a5-9e00-7c6a421a3cf8"

		# Other class vars
		self.is_host = hostMode
		self.bt_sock = None
		self.server_sock = None


	def listen(self):
#     try:
		if(self.server_sock == None):
			# Set up server socket
			self.server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
			self.server_sock.bind(("", self.comm_channel))
			self.server_sock.listen(1)
			bluetooth.advertise_service(self.server_sock,
				self.service_name,
				service_id = self.service_uuid,
				service_classes = [self.service_uuid, bluetooth.SERIAL_PORT_CLASS],
				profiles = [ bluetooth.SERIAL_PORT_PROFILE ], 
#           	protocols = [ OBEX_UUID ] 
			)
			print(">>[RoverConnector.listen] Bluetooth server started, advertising on socket:", self.server_sock.getsockname())

			# Set BT device discoverable
			if(sys.platform.startswith("linux")):
				try:
					dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
					bus = dbus.SystemBus()
					adapter_props = dbus.Interface(bus.get_object('org.bluez', '/org/bluez/hci0'), "org.freedesktop.DBus.Properties")
					adapter_props.Set("org.bluez.Adapter1", "Discoverable", dbus.Boolean(1))
				except: # Try alternate method
					print(">>[RoverConnector.listen] DBus Bluez method failed, using hciconfig to make discoverable.")
					subprocess.call(['sudo', 'hciconfig', 'hci0', 'down'])
					subprocess.call(['sudo', 'hciconfig', 'hci0', 'up'])
					subprocess.call(['sudo', 'hciconfig', 'hci0', 'piscan'])
			else:
				print("!![RoverConnector.listen] Cannot automatically make Bluetooth-discoverable, please do so manually")

		else:
			print(">>[RoverConnector.listen] self.server_sock is not None, ignoring call")
#     except:
#        print("--[RoverConnector.listen] Failed to bind a Bluetooth listener socket!")
#        return False
		return True


	def accept(self, maxTries = 3):
		n = 0
		while True:
			print(">>[RoverConnector.accept] Waiting for a Bluetooth connection (try", n + 1, "of", "%i)" % maxTries)
			try:
				if(self.server_sock == None): self.listen()
				self.bt_sock, client_info = self.server_sock.accept()
				print(">>[RoverConnector.accept] Accepted connection from %s, doing handshake..." % client_info)
				self.doHandshake()
			except:
				print("--[RoverConnector.accept] Connection failed")
				n = n + 1
				if(n >= maxTries):
					print("--[RoverConnector.accept] Reached maximum tries (%i)" % maxTries)
					return False
				else:
					continue

			print("++[RoverConnector.accept] Bluetooth successfully connected and authenticated")
			return True


	def connect(self):
		self.device_mac = self.findDevice()
		# Sometimes None works instead, just letting find_service do its own scanning by UUID
#		self.device_mac = None
		print(">>[RoverConnector.connect] Searching for service...")
		service_matches = bluetooth.find_service( uuid = self.service_uuid, address = self.device_mac )
		if len(service_matches) == 0:
			print("--[RoverConnector.connect] Couldn't find service")
			return False
		first_match = service_matches[0]
		port = first_match["port"]
		name = first_match["name"]
		self.device_mac = first_match["host"]
#		port = 1

		if(self.device_mac == None):
			print("--[RoverConnector.connect] Didn't get a device MAC")
			return False

		print(">>[RoverConnector.connect] Found MAC, attempting to connect to socket...")
#		try:
		self.bt_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
		self.bt_sock.connect((self.device_mac, port)) #self.comm_channel))
		print(">>[RoverConnector.connect] Connected, authenticating socket...")
		self.doHandshake()
		# Tried this but it's slightly more informative to just let exceptions percolate upwards and get stack traced...
#		except:
#			print("--[RoverConnector.connect] Socket connection failed!")
#			return False

		print("++[RoverConnector.connect] Bluetooth successfully connected and authenticated")
		return True


	def send(self, data):
		if(self.bt_sock != None):
#        try:
			self.bt_sock.send(data)
			print("Sent data:", data)
#        except:
#           print("--[RoverConnector.sendData] Failed to send all data")
#           return False
		else:
			print("--[RoverConnector.sendData] Socket not open")
			return False

		return True


	def recv(self, maxLength):
		data = ""
		if(self.bt_sock != None):
#        try:
			data = self.bt_sock.recv(maxLength)
			print("Received data:", data)
#        except: # IOError
#           print("--[RoverConnector.recvData] Error while receiving data")
		else:
			print("--[RoverConnector.recvData] Socket not open")

		return data


	def setBlocking(self, blocking):
		self.bt_sock.setblocking(blocking)


	def disconnect(self):
		if(self.bt_sock != None):
			self.bt_sock.shutdown(2)
			self.bt_sock.close()
			self.bt_sock = None
		if(self.server_sock != None):
			self.server_sock.shutdown(2)
			self.server_sock.close()
			self.server_sock = None
		print(">>[RoverConnector.disconnect] Bluetooth connection closed")


	# *** Internal functions ***

	def findDevice(self):
		bt_addrs = bt_names = { }
		# Calculate the funky power-of-two scan time units and round; discover_devices requires a whole number
		scan_duration = int(round(self.scan_seconds / 1.28))
		self.scan_seconds = scan_duration * 1.28
		print(">>[RoverConnector.findDevice] Scanning Bluetooth for", self.scan_seconds, "seconds (" + str(scan_duration), "rounded duration units)")
		print(">>[RoverConnector.findDevice] Looking for device name: '" + str(self.service_name.encode("utf-8")) + "'")

		# Scan for nearby Bluetooth devices
		for remaining_tries in range(self.scan_tries, -1, -1):
			try:
				# We use a funky catch block because this function call
				#  will often throw an OSError if it doesn't find anything...
				bt_devices = bluetooth.discover_devices(duration = scan_duration, lookup_names = True)
				# ...but just to be safe...
				if(len(bt_devices) == 0): raise Exception

				# Also could do: dev_name = bluetooth.lookup_name(btaddr)
				for device in bt_devices:
					if(self.print_device_names):
						try:
							print("Found device: '" + device[1] + "', with MAC: '" + device[0] + "'")
						except:
							# Sometimes we get Unicode device names but not a compatible console...
							print("Found device (byte-mapped for console compatibility): '" + str(device[1].encode("utf8")) + "', with MAC: '" + device[0] + "'")

					if(str(device[1].encode("utf-8")) == str(self.device_name.encode("utf-8"))):
						print("++[RoverConnector.findDevice] Target device found, MAC: '" + device[0] + "'")
						return device[0]

				# If we didn't find the target device, scan again using some funky looping that's probably much harder than it needs to be
				raise Exception
				
			except:
				if(remaining_tries > 1):
					print("--[RoverConnector.findDevice] Target device name not found, trying up to", remaining_tries - 1, "more time(s)...")
					continue

		print("--[RoverConnector.findDevice] Couldn't find target device!")
		return None


	def doHandshake(self):
		if(self.is_host):
			self.bt_sock.send(self.host_auth_string)
			auth = self.bt_sock.recv(len(self.dev_auth_string))
			if(auth != self.dev_auth_string):
				print("--[RoverConnector.doHandshake] Device failed authentication handshake, instead sent: '%s'" % auth)
				self.bt_sock.close()
				raise Exception
		else:
			self.bt_sock.send(self.dev_auth_string)
			auth = self.bt_sock.recv(len(self.host_auth_string))
			if(auth != self.host_auth_string):
				print("--[RoverConnector.doHandshake] Host failed authentication handshake, instead sent: '%s'" % auth)
				self.bt_sock.close()
				raise Exception


	def runTestMode(self):
		print("*** Welcome to the RoverConnector Bluetooth test mode ***")
		if(self.is_host):
			print("Running in Host mode")
			if(self.connect()):
				print("Sending data...")
				self.send("Hello Bluetooth world!")
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
	rconn = RoverConnectorBluetooth(hostMode)
	rconn.runTestMode()
