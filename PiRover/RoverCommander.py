#!python
#encoding: utf-8

import RoverConnector
import pygame, time
from pygame.locals import *

class RoverCommander():
	# *** Public functions ***
	def __init__(self, connector):
		# *** User variables
		self.wnd_width = 640
		self.wnd_height = 480
		self.window_caption = "RoverCommander"
		self.font_name = pygame.font.get_default_font() # or ex. 'times.ttf'
		self.font_size = 18
		self.line_spacing = 1
		self.antialias = True
		self.text_color = (240, 240, 240)
		self.bg_color = (0, 0, 0)

		# Internal stuff
		pygame.init()
		self.screen = pygame.display.set_mode((self.wnd_width, self.wnd_height))
		self.font = pygame.font.Font(self.font_name, self.font_size)
#		BIGFONT = pygame.font.Font('freesansbold.ttf', 60)
		pygame.display.set_caption(self.window_caption)
		self.max_text_lines = int(self.wnd_height / (self.font_size + self.line_spacing))
		self.current_text = ""
		self.current_command = ""
		self.all_text = []
		self.updateDisplayedText()
		self.is_host = None
		self.rconn = connector
		self.rconn.setBlocking(False)


	def update(self):
#		if(self.checkIncomingMessage() == False): return False
		for event in pygame.event.get():
			if event.type == QUIT:
				self.rconn.send("disconnect")
				return False
			elif event.type == KEYDOWN:
				if event.key == K_ESCAPE:
					self.rconn.send("disconnect")
					return False
				else: self.handleKeyPress(event.key)
			elif event.type == KEYUP:
				self.handleKeyRelease(event.key)

		# Assume we're running in a constant while loop, so don't hog the CPU...
		
		return True


	def runTestMode(self):
		print("*** Welcome to the RoverCommander test mode ***")
		if(isinstance(self.rconn, RoverConnectorBluetooth)):
			# Using real connector
			if(self.is_host):
				print("Running in Host mode")
				if(self.rconn.connect()):
					print("Sending data...")
					self.rconn.send("Hello from RoverCommander!")
					time.sleep(1)
					self.rconn.send("Hello?")
					time.sleep(1)
					self.rconn.send("Anyone?")
					print("Running update loop...")
					while(self.update()): continue
					self.rconn.disconnect()
			else:
				print("Running in Device mode")
				if(self.listen()):
					if(self.accept(2)):
						print("Running update loop...")
						while(self.update()): continue
						self.rconn.disconnect()

		# Using dummy connector
		else:
			print("Running update loop...")
			while(self.update()): continue

		print("*** Script finished")


	# Internal functions
	def handleKeyPress(self, key):
		# Handle arrow keys first to shortcut text updates
		# Obviously because of this there's no line editing other than backspace
		if((key == K_LEFT) or (key == K_RIGHT) or (key == K_UP) or (key == K_DOWN)):
			# Get name, trim 'arrow', send just 'left' / 'right' / 'up' / 'down'
			self.rconn.send(pygame.key.name(key).split()[0])
			return None

		# If not arrow key, treat as text input
		if(key == K_RETURN):
			if(len(self.current_command) > 0):
				# If we add any more of these it might be best to add a checkConsoleCommand()
				# Maybe a 'disconnect' would make sense
				if(self.current_command.lower() == "clear"):
					self.current_command = ""
					self.current_text = ""
					self.all_text = []
				else: 
					self.rconn.send(self.current_command)
					self.current_text += "  (Sent)"
#					self.wrapTextIfNeeded()
					# Can't backspace up to a previous command, so no need to handle it seperately
					self.all_text.append(self.current_text)
					self.current_text = ""
					self.current_command = ""

		elif(key == K_BACKSPACE):
			if(len(self.current_command) > 0):
				self.current_command = self.current_command[:-1]
				if(len(self.current_text) > 0):
					self.current_text = self.current_text[:-1]
				else: # Go back up a line
					self.current_text = self.all_text[-1]
					self.all_text = self.all_text[:-1]
		elif((key >= K_a and key <= K_z) or (key >= K_0 and key <= K_9)):
			all_keys = pygame.key.get_pressed()
			if(all_keys[K_LSHIFT] or all_keys[K_RSHIFT]):
				self.current_command += pygame.key.name(key).upper()
				self.current_text += pygame.key.name(key).upper()
			else:
				self.current_command += pygame.key.name(key)
				self.current_text += pygame.key.name(key)
		elif(key == K_MINUS or key == K_KP_MINUS):
			self.current_command += '-'
			self.current_text += '-'
		elif(key == K_PLUS or key == K_KP_PLUS):
			self.current_command += '+'
			self.current_text += '+'
		self.updateDisplayedText()


	def handleKeyRelease(self, key):
		if((key == K_LEFT) or (key == K_RIGHT) or (key == K_UP) or (key == K_DOWN)):
			# Get name, trim 'arrow', send just 'left' / 'right' / 'up' / 'down' plus '.stop'
			self.rconn.send(pygame.key.name(key).split()[0] + '.stop')


	def wrapTextIfNeeded(self): # NOTE: Operates on current_text, not all_text!
		wrapped_text = ""
		while True:
			# See font.size() doc for why we don't just calculate a max line character width (kerning)
			width, height = self.font.size(self.current_text)
			if(width > self.wnd_width):
				wrapped_text = self.current_text[-1] + wrapped_text
				self.current_text = self.current_text[:-1]
				continue
			else:
				if(len(wrapped_text) > 0):
					self.all_text.append(self.current_text)
					self.current_text = wrapped_text
					wrapped_text = ""
					continue
				else:
					break


	def updateDisplayedText(self):
		self.wrapTextIfNeeded()
		# Shift all lines up if necessary; remember that current_text counts as a line
		current_lines = len(self.all_text) + 1
		if(current_lines > self.max_text_lines):
			lines_to_cut = self.max_text_lines - current_lines
			self.all_text = self.all_text[lines_to_cut:]

		# Clear screen and draw each line
		self.screen.fill(self.bg_color)
		current_lines = len(self.all_text)
		if(current_lines > 0):
			for n in range(0, current_lines):
				rendered_text = self.font.render(self.all_text[n], self.antialias, self.text_color, self.bg_color)
				ypos = n * (self.font_size + self.line_spacing)
				self.screen.blit(rendered_text, (0, ypos))
			n += 1
		else:
			n = 0

		# BUG: If at the very end of a line, the cursor gets cut off rather than moved to the next line
		# Could add it to current_text before wrap then remove it after render, but that sounds like a pain
		rendered_text = self.font.render(self.current_text + "|", self.antialias, self.text_color, self.bg_color)
		ypos = n * (self.font_size + self.line_spacing)
		self.screen.blit(rendered_text, (0, ypos))
		pygame.display.flip()


	def checkIncomingMessage(self):
		msg = self.rconn.recv(128)
		if(len(msg) > 0):
			print("[RoverCommander.checkIncomingMessage] Received message: '%s'" % msg)
			if(msg == "disconnect"):
				return False

		return True


# Test mode using dummy connector
if __name__ == '__main__':
	rconn = RoverConnector(True)
	rcmd = RoverCommander(rconn)
	rcmd.runTestMode()
