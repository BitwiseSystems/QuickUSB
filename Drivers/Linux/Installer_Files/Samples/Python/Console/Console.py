"""============================================================================

Title        : Console.py
Description  : QuickUSB Console App Sample
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


from QuickUsb import *
import sys


print 'Hello QuickUSB World!'

# Find the QuickUSB modules in the system
(ok, modules) = QuickUsb.FindModules()

# Check for no modules and bail if we don't find any
if (len == 0):
	print "Couldn't find any modules\n"
	sys.exit()

# Just use the first device in the list because it's easy
devName = modules[0]

# Print out the name of each module found
for x in modules:
	print "Found", x

# Open the first device
qusb = QuickUsb(devName)
qusb.Open()

# Read the Command register at address 0
length = 2
command = CreateByteBuffer(length)
address = 0
(result, bytes) = qusb.ReadCommand(address, command, length)
if (result == 0):
	print "Cannot read %s command register" % (devName)
	qusb.Close()
	sys.exit()
print "ReadCommand address %d = %04x %04x" % (address, command[0], command[1])

# Make all the pins outputs for now
port = 0
dir = 0xFF
(result,) = qusb.WritePortDir(port, dir)
if (result == 0):
	print "Cannot write %s port A direction register" % (devName)
	qusb.Close()
	sys.exit()

# Loop through a pattern
length = 2
data = CreateByteBuffer(length)
port = 0
print "Writing 128 cycles of 0xAAAA, 0x5555 to %s, port A..." % (devName)
for count in xrange(128):
	data = 0xAAAA
	(result,) = qusb.WritePort(port, data, length)
	if (result == 0):
		print "Cannot write %s port A data" % (devName)
		qusb.Close()
		sys.exit()

	data = 0x5555
	(result,) = qusb.WritePort(port, data, length)
	if (result == 0):
		print "Cannot write %s port A data" % (devName)
		qusb.Close()
		sys.exit()

# Write a 4k block of data
length = 4096
bulkdata = CreateByteBuffer(length)
for i in xrange(length):
	bulkdata[i] = i%256
(result,) = qusb.WriteData(bulkdata, length)
if (result == 0):
	print "Cannot write data to %s" % (devName)
	qusb.Close()
	sys.exit()
print "QuickUsbWriteData wrote %d bytes to %s" % (length, devName)

# Close the port when you're done with it
qusb.Close()

print "Done"
