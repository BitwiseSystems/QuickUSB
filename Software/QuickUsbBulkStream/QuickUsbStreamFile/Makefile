CPP=g++
CPPFLAGS=-Wall -lc -lquickusb

all: QuickUsbStreamFile.cpp
	$(CPP) $(CPPFLAGS) -lquickusb -o QuickUsbStreamFile QuickUsbStreamFile.cpp

release: all

debug: QuickUsbStreamFile.cpp
	$(CPP) -g $(CPPFLAGS) -lquickusb -o QuickUsbStreamFile QuickUsbStreamFile.cpp
