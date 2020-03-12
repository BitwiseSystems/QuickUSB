"""============================================================================

Title        : storage_test.py
Description  : QuickUSB storage test
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
#		3. Run script with "python storage_test.py"


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


# QuickUsb Storage
print 'QuickUsb Storage'

# Create byte buffer. The size should not exceed 2k.
size = 64
data = QuickUsb.CreateByteBuffer(size)

print '\nReadStorage'
print '\tData should be the last written values'
(ok,) = qusb.ReadStorage(0, data, size)
print '\tData:', data[:]

print '\nWriteStorage'
for i in xrange(size):
	data[i] = i
(ok,) = qusb.WriteStorage(0, data, size)
print '\tReturn:', ok

print 'ReadStorage'
print '\tData should be sequential values from 0 to 63'
(ok,) = qusb.ReadStorage(0, data, size)
print '\tData:', data[:]

print '\nWriteStorage'
for i in xrange(size):
	data[i] = size-i-1
(ok,) = qusb.WriteStorage(0, data, size)
print '\tReturn:', ok

print 'ReadStorage'
print '\tData should be sequential values from 63 to 0'
(ok,) = qusb.ReadStorage(0, data, size)
print '\tData:', data[:]

print '\nPower cycle the module, rerun this test, and make sure the first read is similar to the last read.'


qusb.Close()
