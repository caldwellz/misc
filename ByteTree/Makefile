
TARGETDIR = .
INCLUDEDIR = $(TARGETDIR)/include
SRCDIR = $(TARGETDIR)/src
OBJDIR = $(TARGETDIR)/obj
BINDIR = $(TARGETDIR)/bin

CC = clang
CXX = clang++
CFLAGS = -O2 -s -I$(INCLUDEDIR)
CFLAGSD = -O0 -g -I$(INCLUDEDIR)

DEBUG_TARGETS = $(BINDIR)/test_cbtd.exe $(BINDIR)/test_mapd.exe
RELEASE_TARGETS = $(BINDIR)/test_cbt.exe $(BINDIR)/test_map.exe

HEADERS_CBT = $(INCLUDEDIR)/PropertyTree.hxx $(INCLUDEDIR)/PropertyTree_bytetree.hxx $(INCLUDEDIR)/bytetree.h
OBJS_CBT = $(OBJDIR)/Test_cbt.oxx $(OBJDIR)/bytetree.o $(OBJDIR)/PropertyTree_bytetree.oxx 
OBJS_CBT_D = $(OBJDIR)/Test_cbt.oxxd $(OBJDIR)/bytetree.od $(OBJDIR)/PropertyTree_bytetree.oxxd 
HEADERS_MAP = $(INCLUDEDIR)/PropertyTree.hxx $(INCLUDEDIR)/PropertyTree_map.hxx
OBJS_MAP = $(OBJDIR)/Test_map.oxx $(OBJDIR)/PropertyTree_map.oxx 
OBJS_MAP_D = $(OBJDIR)/Test_map.oxxd $(OBJDIR)/PropertyTree_map.oxxd 


all: build
	echo Finished target 'all'.

build: $(RELEASE_TARGETS) $(DEBUG_TARGETS)
	echo Finished target 'build'.

clean:
	rm bin/*.exe
	rm obj/*.o*

$(OBJDIR)/Test_cbt.o*: $(SRCDIR)/Test.cxx
$(OBJDIR)/Test_map.o*: $(SRCDIR)/Test.cxx

$(OBJDIR)/%.oxx: $(SRCDIR)/%.cxx
	$(CXX) $(CFLAGS) -std=c++11 -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.oxxd: $(SRCDIR)/%.cxx
	$(CXX) $(CFLAGSD) -std=c++11 -c -o $@ $<

$(OBJDIR)/%.od: $(SRCDIR)/%.c
	$(CC) $(CFLAGSD) -c -o $@ $<

$(BINDIR)/test_cbt.exe: $(OBJS_CBT) $(HEADERS_CBT)
	$(CXX) -s -o $@ $(OBJS_CBT)

$(BINDIR)/test_cbtd.exe: $(OBJS_CBT_D) $(HEADERS_CBT)
	$(CXX) -o $@ $(OBJS_CBT_D)

$(BINDIR)/test_map.exe: $(OBJS_MAP) $(HEADERS_MAP)
	$(CXX) -s -o $@ $(OBJS_MAP)

$(BINDIR)/test_mapd.exe: $(OBJS_MAP_D) $(HEADERS_MAP)
	$(CXX) -o $@ $(OBJS_MAP_D)
