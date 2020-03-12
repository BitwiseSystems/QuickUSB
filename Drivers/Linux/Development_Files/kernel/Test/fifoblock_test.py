"""============================================================================

Title        : fifoblock_test.py
Description  : QuickUSB FIFO block test
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
#		3. Run script with "python fifoblock_test.py <value>"
#			a. Write: value == 1
#			b. Read: value == 0
#       4. Use jumper wires to ground the following pins:
#           a. RDY0: nEmpty
#           b. RDY1: nFull


import QuickUsb
import sys


# Check for the correct usage
if (len(sys.argv) != 2):
    print 'Usage: python fifoblock_test.py <value>'
    print '\tvalue(0): Read'
    print '\tvalue(1): Write'
    sys.exit()


# Find modules
(ok, modules) = QuickUsb.QuickUsb.FindModules()
if (len(modules) == 0):
	print 'No QuickUSB modules'
	sys.exit()

# Open QuickUSB module
qusb = QuickUsb.QuickUsb(modules[0])
qusb.Open()


print 'QuickUSB FIFO block'
print '\tRDY0 - nEMPTY'
print '\tRDY1 - nFULL'

isWrite = int(sys.argv[1])
size = 512
data = QuickUsb.CreateByteBuffer(size)
if (isWrite == 1):
    print '\nWrite'
    print '\tGrounded RDY0 should succeed.'
    print '\tGrounded RDY1 should cause a timeout error.'
    # Note: Observations of the Windows driver behavior...
    #       Writing 1024 words would fail the first time.
    #       Writing 512  words would succeed the first time, but fail the second time.
    #       Writing 256  words would succeed the first two times, but fail the third time.
    #       Writing 128  words would succeed the first two times, but fail the third time.
    #       Writing 64   words would succeed the first two times, but fail the third time.
    (ok,) = qusb.WriteData(data, size)
else:
    print '\nRead'
    print '\tGrounded RDY0 should cause a timeout error.'
    print '\tGrounded RDY1 should succeed.' 
    (ok, bytes) = qusb.ReadData(data, size)

print '\nIf the module timeouts, power cycle the module before performing additional tests.'


qusb.Close()

