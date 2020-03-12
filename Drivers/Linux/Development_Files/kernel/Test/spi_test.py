"""============================================================================

Title        : spi_test.py
Description  : QuickUSB SPI test
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
#		3. Run script with "python spi_test.py"
#		4. Verify output with logic analyzer
#			a. PE0 - MOSI
#			b. PE1 - SCLK


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


# SPI-Compatible Port
print 'SPI-Compatible Port'
print '\tPE0 - data(MOSI)'
print '\tPE1 - clock'

print 'WriteSpi'
print '\tMOSI should equal 0 1010011 0 1010000 0 1001001 on the rising edges of the clock.'
size = 3
data = QuickUsb.CreateByteBuffer(size)
data[0] = 0x53
data[1] = 0x50
data[2] = 0x49
(ok,) = qusb.WriteSpi(0, data, size)
print '\tReturn:', ok

print 'ReadSpi'
(ok, bytes) = qusb.ReadSpi(0, data, size)
print '\tData:', data[:]

print 'WriteReadSpi'
print '\tMOSI should equal 0 1010011 0 1010000 0 1001001 on the rising edges of the clock.'
data[0] = 0x53
data[1] = 0x50
data[2] = 0x49
qusb.WriteReadSpi(0, data, 3)
print '\tData:', data[:]


qusb.Close()

