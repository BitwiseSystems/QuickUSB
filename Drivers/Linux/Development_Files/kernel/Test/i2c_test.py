"""============================================================================

Title        : i2c_test.py
Description  : QuickUSB I2C test
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
#		3. Run script with "python i2c_test.py"
#		4. Verify output with logic analyzer.
#			a. SCL
#			b. SDA


import QuickUsb
import sys


# Find modules
(ok, modules) = QuickUsb.QuickUsb.FindModules()
if (len(modules) == 0):
	print 'No QuickUSB modules'
	sys.exit()

# Open QuickUSB module
qusb = QuickUsb.QuickUsb(modules[0])
qusb.Open()


# I2C-Compatible Port
print 'I2C-Compatible Port'
print '\tSCL - clock'
print '\tSDA - data'

# Write I2C
print '\nWriteI2C'
print '\tSet to ignore ack: Bit 7 should be high.'
(ok, i2ctl) = qusb.ReadSetting(QuickUsb.Setting.I2Ctl)
i2ctl = i2ctl|1<<7
(ok,) = qusb.WriteSetting(QuickUsb.Setting.I2Ctl, i2ctl)
(ok, i2ctl) = qusb.ReadSetting(QuickUsb.Setting.I2Ctl)
print '\t\tI2Ctl:', hex(i2ctl)

print '\tWrite data'
print '\tSDA should be 10 1001001 10 0110010 10 1000011 10 on the rising edges of the clock.'
size = 3
data = QuickUsb.CreateByteBuffer(size)
data[0] = 0x49
data[1] = 0x32
data[2] = 0x43
(ok,) = qusb.WriteI2C(0, data, size)

# Read I2C
# This needs to be properly tested.
print 'ReadI2C'
(ok, bytes) = qusb.ReadI2C(0, data, size)
print '\tData:', data[:]


qusb.Close()

