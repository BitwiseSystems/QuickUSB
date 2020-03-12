"""============================================================================

Title        : gpio_test.py
Description  : QuickUSB GPIO test
Notes        : None
History      :

 Copyright (c) 2020 Bitwise Systems, Inc.

 Permission is hereby granted, free of charge, to any person obtaining 
 a copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation 
 the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the 
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included 
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 IN THE SOFTWARE.


============================================================================"""



#	Execute:
#		1. Navigate in a terminal to the directory with this script.
#		2. Ensure the latest QuickUsb.py is present in the directory.
#		3. Run script with "python gpio_test.py"


import QuickUsb
import sys
import time


# Find modules
(ok, modules) = QuickUsb.QuickUsb.FindModules()
if (len(modules) == 0):
	print 'No QuickUSB modules'
	sys.exit()

# Open QuickUSB module
qusb = QuickUsb.QuickUsb(modules[0])
qusb.Open()


# General-Purpose I/O
print 'General-Purpose I/O'

# If you change the following port number, you must also change the port for ReadSetting.
port = 0


print '\nThe following test sets the port directions as output and toggles the port values'
print '\tWritePortDir'
t = 0xFF;
(ok,) = qusb.WritePortDir(port, t)

print '\tReadPortDir: The port direction should read 0xff'
(ok, portdir) = qusb.ReadPortDir(port)
print '\t\tPort direction:', hex(portdir)

print '\tWritePort'
portval = QuickUsb.CreateByteBuffer(1)
portval[0] = 0xAA
(ok,) = qusb.WritePort(port, portval, 1)

print '\tReadPort: The port values should read 0xaa'
(ok, r) = qusb.ReadPort(port, portval, 1)
print '\t\tPort values:', hex(portval[0])

print '\tRead port setting: The port setting should read 0xffaa'
(ok, portsetting) = qusb.ReadSetting(QuickUsb.Setting.PortA)
print '\t\tPort setting:', hex(portsetting)

print '\tWritePort'
portval[0] = 0x55
(ok,) = qusb.WritePort(port, portval, 1)

print '\tReadPort: The port values should read 0x55'
(ok, r) = qusb.ReadPort(port, portval, 1)
print '\t\tPort values:', hex(portval[0])

print '\tRead port setting: The port setting should read 0xff55'
(ok, portsetting) = qusb.ReadSetting(QuickUsb.Setting.PortA)
print '\t\tPort setting:', hex(portsetting)


print '\nThe following test sets the port values to high and toggles the port direction'
print '\tWritePort'
portval[0] = 0xFF
(ok,) = qusb.WritePort(port, portval, 1)

print '\tReadPort: The port value should read 0xff'
(ok, r) = qusb.ReadPort(port, portval, 1)
print '\t\tPort value:', hex(portval[0])

print '\tWritePortDir'
t = 0xAA;
(ok,) = qusb.WritePortDir(port, t)

print '\tReadPortDir: The port direction should read 0xaa'
(ok, portdir) = qusb.ReadPortDir(port)
print '\t\tPort direction:', hex(portdir)

print '\tRead port setting: The port setting should read 0xaa-- where -- could vary.\n\tThe port values should at least have bits 7, 5, 3, and 1 set to high.'
(ok, portsetting) = qusb.ReadSetting(QuickUsb.Setting.PortA)
print '\t\tPort setting:', hex(portsetting)

print '\tWritePortDir'
t = 0x55;
(ok,) = qusb.WritePortDir(port, t)

print '\tReadPortDir: The port direction should read 0x55'
(ok, portdir) = qusb.ReadPortDir(port)
print '\t\tPort direction:', hex(portdir)

print '\tRead port setting: The port setting should read 0x55-- where -- could vary.\n\tThe port values should at least have bits 6, 4, 2, and 0 set to high.'
(ok, portsetting) = qusb.ReadSetting(QuickUsb.Setting.PortA)
print '\t\tPort setting:', hex(portsetting)


qusb.Close()

