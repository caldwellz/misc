CC=g++
MKDIR=mkdir
RELFLAGS=-O3 -ffast-math -fomit-frame-pointer
DEBUGFLAGS=-O0 -g

all: rubber.exe
debug: rubber-d.exe

rubber.exe: obj/Release/Rubber.o obj/Release/MasterNode.o obj/Release/Node.o
	g++ -Wl,-s -o $@ $^ -lenet -lws2_32 -lwinmm

obj/Release/Rubber.o: src/Rubber.cpp obj/Release
	$(CC) $(RELFLAGS) -c -o $@ $^

obj/Release/MasterNode.o: src/MasterNode.cpp obj/Release
	$(CC) $(RELFLAGS) -c -o $@ $^

obj/Release/Node.o: src/Node.cpp obj/Release
	$(CC) $(RELFLAGS) -c -o $@ $^

rubber-d.exe: obj/Debug/Rubber.o obj/Debug/MasterNode.o obj/Debug/Node.o
	g++ -o $@ $^ -lenet -lws2_32 -lwinmm

obj/Debug/Rubber.o: src/Rubber.cpp obj/Debug
	$(CC) $(DEBUGFLAGS) -c -o $@ $^

obj/Debug/MasterNode.o: src/MasterNode.cpp obj/Debug
	$(CC) $(DEBUGFLAGS) -c -o $@ $^

obj/Debug/Node.o: src/Node.cpp obj/Debug
	$(CC) $(DEBUGFLAGS) -c -o $@ $^

obj:
	-$(MKDIR) $@

obj/Release: obj
	-$(MKDIR) $@

obj/Debug: obj
	-$(MKDIR) $@
