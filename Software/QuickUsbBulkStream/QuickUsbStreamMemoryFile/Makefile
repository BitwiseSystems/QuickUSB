CPP=g++
CPPFLAGS=-Wall -lc -lquickusb

all: QuickUsbStreamMemoryFile.cpp
	$(CPP) $(CPPFLAGS) -lquickusb -o QuickUsbStreamMemoryFile QuickUsbStreamMemoryFile.cpp

release: all

debug: QuickUsbStreamMemoryFile.cpp
	$(CPP) -g $(CPPFLAGS) -lquickusb -o QuickUsbStreamMemoryFile QuickUsbStreamMemoryFile.cpp
