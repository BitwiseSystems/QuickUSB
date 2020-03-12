MinGW

To compile for MinGW using the command line, type
make -f Makefile.win

Linux

To compile for Linux using the command line, type
make -f Makefile.lnx ARCH=32
make -f Makefile.lnx ARCH=64

Mac
http://developer.apple.com/library/mac/#documentation/DeviceDrivers/Conceptual/USBBook/USBDeviceInterfaces/USBDevInterfaces.html

To compile for Mac using Xcode,

Release:

1. In Xcode, Project > Set Active Build Configuration > Release
2. Right click "Copy to QuickUSB universal" in the left panel under Targets > QuickUSB and select Get Info
3. Change the full path to...

/Projects/Bitwise/QuickUSB/Library/Software/QuickUSB/universal/Release/Mac

4. Close the Copy Files Phase for "QuickUSB" Info window
5. Project > Edit Active Target "QuickUSB"
6. Select Build at the top
7. Select Release in the Configuration dropbox near the top
8. Make sure Skip Install has a checkbox
9. Close the Target "QuickUSB" Info window
10. Build > Build
NOTE: The library will be built in ./build and also copied to ./universal/Release/Mac/. If you want to install the library to /usr/local/lib/, follow the instructions below using the command line.

Debug: 
1. In Xcode, Project > Set Active Build Configuration > Debug
2. Right click Copy Files in the left panel under Targets > QuickUSB and select Get Info
3. Change the full path to...

/Projects/Bitwise/QuickUSB/Library/Software/QuickUSB/universal/Debug/Mac

4. Close the Copy Files Phase for "QuickUSB" Info window
5. Project > Edit Active Target "QuickUSB"
6. Select Build at the top
7. Select Debug in the Configuration dropbox near the top
8. Make sure Skip Install has a checkbox
9. Close the Target "QuickUSB" Info window
10. Build > Build
NOTE: The library will be built in ./build and also copied to ./universal/Debug/Mac/. If you want to install the library to /usr/local/lib, follow the instructions below using the command line.

To compile for Mac using the command line,
 
Release:
1. In Xcode, Project > Edit Active Target "QuickUSB"
2. Select Build at the top
3. Select Release in the Configuration dropbox near the top
4. Make sure /usr/local/lib is set for the Installation Directory
5. Make sure Skip Install has a blank checkbox
6. Close the Target "QuickUSB" Info window
7. In a terminal, navigate to...

/Projects/Bitwise/QuickUSB/Library/Software/QuickUSB/

8. Type...

sudo make -f Makefile.mac

Debug:
1. In Xcode, Project > Edit Active Target "QuickUSB"
2. Select Build at the top
3. Select Debug in the Configuration dropbox near the top
4. Make sure /usr/local/lib is set for the Installation Directory
5. Make sure Skip Install has a blank checkbox
6. Close the Target "QuickUSB" Info window
7. In a terminal, navigate to...

/Projects/Bitwise/QuickUSB/Library/Software/QuickUSB/

8. Type...

sudo make -f Makefile.mac

Clean:

sudo make -f Makefile.mac clean

Uninstall:

sudo make -f Makefile.mac uninstall
