#!python
	
import wiringpi, time

class RoverController():
	# *** Public functions ***
	def __init__(self, connector):
		# *** User variables ***
		self.drive_motor_forward_pin = 5
		self.drive_motor_reverse_pin = 6
		self.turn_motor_left_pin = 2
		self.turn_motor_right_pin = 3
		self.pwm0_pin = 1
		self.clock_pin = 7
		self.led_one_pin = 4
		# Serial depends on how PyBluez sets it up, but it's usually this
		self.serial_dev = "/dev/ttyAMA0"
		self.serial_baud = 9600

		# Internal setup
		self.rc = connector
		self.rc.setBlocking(True)
		wiringpi.wiringPiSetup()
		self.serial = wiringpi.serialOpen(self.serial_dev, self.serial_baud)
		wiringpi.pinMode(self.drive_motor_forward_pin, 1)
		wiringpi.pinMode(self.drive_motor_reverse_pin, 1)
		wiringpi.pinMode(self.turn_motor_left_pin, 1)
		wiringpi.pinMode(self.turn_motor_right_pin, 1)

		# Just output a constant high on drive PWM for now...
		wiringpi.pinMode(self.pwm0_pin, 1)
		wiringpi.digitalWrite(self.pwm0_pin, 1)


	def update(self):
		data = self.rc.recv(128)
		if(data == "disconnect"):
			self.processCommand(cmd, None)
			return False
		elif(len(data) > 0):
			msg = data.split()
			cmd = msg[0]
			if(len(msg) > 1):
				self.processCommand(cmd, msg[1:])
			else:
				self.processCommand(cmd, None)
		return True


	def processCommand(self, cmd, args = ""):
		# Only reason this uses 'up' and 'down' is because Commander is sending
		# arrow key names from PyGame, and I'm too lazy to add special cases
		# there right now just to rewrite those to 'forward' and 'reverse'...
		if(cmd == b"up"): self.forward()
		elif(cmd == b"up.stop"): self.stopForward()
		elif(cmd == b"left"): self.turnLeft()
		elif(cmd == b"left.stop"): self.stopLeft()
		elif(cmd == b"right"): self.turnRight()
		elif(cmd == b"right.stop"): self.stopRight()
		elif(cmd == b"down"): self.reverse()
		elif(cmd == b"down.stop"): self.stopReverse()
		elif(cmd == b"all.stop"): self.stopAll()
		elif(cmd == b"led1.on"): self.setLEDOne(wiringpi.PUD_DOWN)
		elif(cmd == b"led1.off"): self.setLEDOne(wiringpi.PUD_OFF)
		# Main should handle the actual network disconnect
		elif(cmd == b"disconnect"): self.stopAll()


	def forward(self):
		wiringpi.digitalWrite(self.drive_motor_reverse_pin, 0)
		# Maybe sleep here depending on need / whether previously in reverse
		wiringpi.digitalWrite(self.drive_motor_forward_pin, 1)


	# Only reason to keep this and stopReverse separate might be to apply quick-stop pulses...
	def stopForward(self):
		wiringpi.digitalWrite(self.drive_motor_forward_pin, 0)
#		wiringpi.digitalWrite(self.drive_motor_reverse_pin, 1)
#		time.sleep(0.1)
		wiringpi.digitalWrite(self.drive_motor_reverse_pin, 0)


	def reverse(self):
		wiringpi.digitalWrite(self.drive_motor_forward_pin, 0)
		wiringpi.digitalWrite(self.drive_motor_reverse_pin, 1)


	def stopReverse(self):
		wiringpi.digitalWrite(self.drive_motor_reverse_pin, 0)
#		wiringpi.digitalWrite(self.drive_motor_forward_pin, 1)
#		time.sleep(0.1)
		wiringpi.digitalWrite(self.drive_motor_forward_pin, 0)


	def turnLeft(self):
		wiringpi.digitalWrite(self.turn_motor_right_pin, 0)
		wiringpi.digitalWrite(self.turn_motor_left_pin, 1)


	def stopLeft(self):
		wiringpi.digitalWrite(self.turn_motor_right_pin, 0)
		wiringpi.digitalWrite(self.turn_motor_left_pin, 0)


	def turnRight(self):
		wiringpi.digitalWrite(self.turn_motor_left_pin, 0)
		wiringpi.digitalWrite(self.turn_motor_right_pin, 1)


	def stopRight(self):
		wiringpi.digitalWrite(self.turn_motor_left_pin, 0)
		wiringpi.digitalWrite(self.turn_motor_right_pin, 0)


	def stopAll(self):
		wiringpi.digitalWrite(self.turn_motor_left_pin, 0)
		wiringpi.digitalWrite(self.turn_motor_right_pin, 0)
		wiringpi.digitalWrite(self.drive_motor_forward_pin, 0)
		wiringpi.digitalWrite(self.drive_motor_reverse_pin, 0)

	def setLEDOne(self, value):
		wiringpi.pullUpDnControl(self.led_one_pin, value)
