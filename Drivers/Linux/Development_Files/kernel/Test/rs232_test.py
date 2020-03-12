"""============================================================================

Title        : rs232_test.py
Description  : QuickUSB RS232 test
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
#		3. Attach a loopback RS-232 cable to the two RS-232 ports of the evaluation board.
#		4. Run script with "python rs232_test.py".
#		5. Verify baud rate with a logic analyzer.
#			a. RXD_0
#			b. TXD_0
#			c. RXD_1
#			d. TXD_1
#
#	Results:
#		1. TXD0 and RXD1 should switch faster at a baud rate of 230000 than 4800


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


# RS-232
print 'RS-232'
print '\tRXD_0 - Receive port 0'
print '\tTXD_0 - Transmit port 0'
print '\tRXD_1 - Receive port 1'
print '\tTXD_1 - Transmit port 1'

print '\nThis test sets the baud rate to 4800, writes data to port 0, and reads data from port 1.'
print '\tSetRS232BaudRate to 4800'
(ok,) = qusb.SetRS232BaudRate(4800)
print '\t\tReturn:', ok

print '\tFlushRS232 both ports'
(ok,) = qusb.FlushRS232(0)
print '\t\tReturn 0:', ok
(ok,) = qusb.FlushRS232(1)
print '\t\tReturn 1:', ok

print '\tWriteRS232 to port 0'
size = 64
wdata = QuickUsb.CreateByteBuffer(size)
for i in xrange(size):
	wdata[i] = i
(ok,) = qusb.WriteRS232(0, wdata, size)
print '\t\tReturn:', ok

print '\tGetNumRS232: Number of characters should be 64.'
time.sleep(0.2)
(ok, numchar) = qusb.GetNumRS232(1)
print '\t\tNumber of characters:', numchar

if numchar > 0:
	print '\tReadRS232: Data should be sequential values from 0 to 63'
	rdata = QuickUsb.CreateByteBuffer(numchar)
	(ok, bytes) = qusb.ReadRS232(1, rdata, numchar)
	print '\t\tData:', rdata[:]


print '\nThis test sets the baud rate to 230000, writes data to port 1, and reads data from port 2.'
print '\tSetRS232BaudRate to 230000'
(ok,) = qusb.SetRS232BaudRate(230000)
print '\t\tReturn:', ok

print '\tFlushRS232 both ports'
(ok,) = qusb.FlushRS232(0)
print '\t\tReturn 0:', ok
(ok,) = qusb.FlushRS232(1)
print '\t\tReturn 1:', ok

print '\tWriteRS232 to port 1'
size = 64
wdata = QuickUsb.CreateByteBuffer(size)
for i in xrange(size):
	wdata[i] = size - i - 1
(ok,) = qusb.WriteRS232(1, wdata, size)
print '\t\tReturn:', ok

print '\tGetNumRS232: Number of characters should be 64.'
time.sleep(0.2)
(ok, numchar) = qusb.GetNumRS232(0)
print '\t\tNumber of characters:', numchar

if numchar > 0:
	print '\tReadRS232: Data should be sequential values from 63 to 0'
	(ok, bytes) = qusb.ReadRS232(0, rdata, numchar)
	print '\t\tData:', rdata[:]



qusb.Close()

