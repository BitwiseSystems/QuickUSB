CPPFLAGS := $(CPPFLAGS) -g -fpic
LIBDIR := /usr/local/lib

all: Console.cpp
	gcc -o Console -Wall Console.cpp /usr/lib/libquickusb.so.2 -lusb -lstdc++
