#!python3

import os, string

def addListItem(dirpath, fn):
	if not hasattr(addListItem, "curdir"):
		addListItem.curdir = ""  # it doesn't exist yet, so initialize it
	if not hasattr(addListItem, "counter"):
		addListItem.counter = 0

	addListItem.counter += 1
	if not (dirpath == addListItem.curdir):
		print(dirpath)
		addListItem.curdir = dirpath

	print('\t' + format(addListItem.counter, '03d') + ":", fn)


def main():
#	basepath = "C:\AMD"
	while True:
		basepath = input("Base path?> ")
		if(os.path.exists(basepath)): break
		print("\nPath not accessible, please try again...")

	for root, dirs, files in os.walk(basepath):
		for file in files:
			addListItem(os.path.relpath(root, basepath), file)			

	print("Done,", str(addListItem.counter), "items listed.")


main()
