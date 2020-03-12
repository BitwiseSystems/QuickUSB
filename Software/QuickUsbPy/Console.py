"""============================================================================

Title        : Console.py
Description  : QuickUSB Console App Sample
Notes        : None
History      :

Copyright  2012 Bitwise Systems.  All rights reserved.
This software contains confidential information and trade secrets of
Bitwise Systems and is protected by United States and international
copyright laws.  Use, disclosure, or reproduction is prohibited without
the prior express written permission of Bitwise Systems, except as agreed
in the QuickUSB Plug-In Module license agreement.

Use, duplication or disclosure by the U.S. Government is subject to
restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
(1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real, 
Suite E, Goleta, CA  93117.

Bitwise Systems
6489 Calle Real, Suite E
Goleta, CA  93117
Voice: (805) 683-6469
Fax  : (805) 683-4833
Web  : www.bitwisesys.com
email: support@bitwisesys.com

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
