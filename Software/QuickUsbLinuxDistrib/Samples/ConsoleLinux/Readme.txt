This is a console app example for QuickUSB.  It shows how to read the list of attached modules, open the first one and execute some commands.  Of course, once the port is open you can call any of the QuickUSB Library commands as documented in the User Guide.

And because this is a console application, it will compile and run on any supported platform.  This example is mainly intended for Windows under Visual Studio or MinGW under Bloodshed DevC++, but it's simple enough to adapt for many other uses.

Project files:
console.cpp		C++ source code for the example
console.dev		MinGW Project file for Bloodshed Dev C++ 4.9.9.2 (www.bloodshed.net)
console.dsp/dsw	Visual Studio 6.0 Project files (Can be upgraded using Visual Studio)
console.sln		Visual Studio 8.0 Project file
libquickusb.a	The QuickUSB Linkable Library for MinGW (links to QuickUSB.DLL)
quickusb.dll	The QuickUSB DLL (This file should normally reside in \Windows\System32
quickusb.h		The QuickUSB Header File
quickusb.lib	The QuickUSB Linkable Library (links to QuickUSB.DLL)

Enjoy!