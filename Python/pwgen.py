#!python

import string
import random
#import win32clipboard

secrandom = random.SystemRandom()
# 20 character passwords
lowerChars = 5
upperChars = 5
specChars = 5
numbers = 5
length = lowerChars + upperChars + specChars + numbers

def genPW():
	lowerCharsList = []
	upperCharsList = []
	specCharsList = []
	numbersList = []
	pwArray = []
	pw = ""

	# Create character dictionaries
	for char in string.ascii_lowercase:
		lowerCharsList.append(char)
	for char in string.ascii_uppercase:
		upperCharsList.append(char)
	for char in string.punctuation:
		specCharsList.append(char)
	specCharsList.remove('\'')
	specCharsList.remove('`')
	specCharsList.remove('"')
	specCharsList.remove('\\')
	specCharsList.remove('|')
	for num in string.digits:
		numbersList.append(num)

	# Construct the password
	for n in range(0, lowerChars):
		pwArray.append(secrandom.choice(lowerCharsList))
	for n in range(0, upperChars):
		pwArray.append(secrandom.choice(upperCharsList))
	for n in range(0, specChars):
		pwArray.append(secrandom.choice(specCharsList))
	for n in range(0, numbers):
		pwArray.append(secrandom.choice(numbersList))

	# Randomize the password
	for n in range(0, length):
		char = secrandom.choice(pwArray)
		pw += char
		pwArray.remove(char)

#	print(lowerCharsList)
#	print(upperCharsList)
#	print(specCharsList)
#	print(numbersList)
#	print(pwArray)
#	print(pw)
	return pw

if __name__ == '__main__':
	pw = genPW()
	print(pw)
#	win32clipboard.OpenClipboard()
#	win32clipboard.EmptyClipboard()
#	win32clipboard.SetClipboardText(pw)
#	win32clipboard.CloseClipboard()
#	genPW()
#	genPW()
	pause = input()
