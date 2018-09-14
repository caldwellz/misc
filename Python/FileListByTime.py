#!python3

import os, string

def addListItem(basepath, dirpath, fn):
	if not hasattr(addListItem, "curdir"):
		addListItem.curdir = ""  # it doesn't exist yet, so initialize it
	if not hasattr(addListItem, "counter"):
		addListItem.counter = 0
	if not hasattr(addListItem, "outfile"):
		addListItem.outfile = open("file-list.txt", 'w')

	addListItem.counter += 1
	if not (dirpath == addListItem.curdir):
		addListItem.outfile.write(os.path.relpath(dirpath, basepath) + '\n')
		addListItem.curdir = dirpath

	addListItem.outfile.write('\t' + format(addListItem.counter, '03d') + ": " + fn + '\n')


def main():
#	basepath = "C:\AMD"
	while True:
		basepath = input("Base path?> ")
		if(os.path.exists(basepath)): break
		print("\nPath not accessible, please try again...")

	filelist = []
	for root, dirs, files in os.walk(basepath):
		for file in files:
			filelist.append(os.path.join(root, file))

	filelist.sort(key=lambda f: os.path.getmtime(f))
	for file in filelist:
		addListItem(basepath, *os.path.split(file))			

	print("Done -", str(addListItem.counter), "items listed.")
	addListItem.outfile.close()


main()
