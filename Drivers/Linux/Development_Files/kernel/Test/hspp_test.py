"""============================================================================

Title        : hspp_test.py
Description  : QuickUSB HSPP test
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
#		3. Run script with "python hspp_test.py <value>"


import QuickUsb
import sys
import time


if (len(sys.argv) == 2):
	size = int(sys.argv[1])
else:
    print 'Usage: python hspp_test.py <value>'
    print '\tvalue: Packet size'
    sys.exit()


# No exceptions
QuickUsb.QuickUsb.SetThrowExceptions(True);


# Find modules
(ok, modules) = QuickUsb.QuickUsb.FindModules()
if (len(modules) == 0):
	print 'No QuickUSB modules'
	sys.exit()

# Open QuickUSB module
qusb = QuickUsb.QuickUsb(modules[0])
qusb.Open()


# HSPP
print 'High-Speed Parallel Port'
print '\tPB[0-7] - FD[7-0]'
print '\tPD[0-7] - FD[15-8]'
print '\tCTL0 - CMD_DATA'
print '\tCTL1 - REN'
print '\tCTL2 - WEN'
print '\tCTL3 - nREN'
print '\tCTL4 - nWEN'
print '\tCTL5 - nOE'
print '\tRDY0 - nEMPTY'
print '\tRDY1 - nFULL'

# Check if FPGA is configured
print '\nIs FPGA Configured'
(ok, configured) = qusb.IsFpgaConfigured()
print '\tConfigured:', configured

if configured == 0:
	# Turn on FPGA power and reset FPGA logic
	print 'Reset FPGA logic'
	print '\tPort A setting should read 0x81-- where -- could vary, but at least bit 7 and 0 should be high.'
	val = QuickUsb.CreateByteBuffer(1)
	val = 0x80
	(ok,) = qusb.WritePortDir(0, 0x81) # Set port A bit 7 and 0 as outputs
	(ok,) = qusb.WritePort(0, val, 1) # Set port A bit 7 high and bit 0 low
	val = 0x81
	(ok,) = qusb.WritePort(0, val, 1) # Set port A bit 0 high
	(ok, r) = qusb.ReadSetting(QuickUsb.Setting.PortA)
	print '\tPort A setting:', hex(r)

	# Set wordwide
	print 'Set word wide'
	print '\tWord wide should read 0x1.'
	(ok,) = qusb.WriteSetting(QuickUsb.Setting.WordWide, 1)
	(ok, word_wide) = qusb.ReadSetting(QuickUsb.Setting.WordWide)
	print '\tWord wide:', hex(word_wide)
	
	# Set fifo configuration
	print 'Set fifo configuration'
	print '\tFifo configuration should at least have bit 1 set high and bit 0 set low.'
	(ok, fifo_config) = qusb.ReadSetting(QuickUsb.Setting.FifoConfig)
	t = fifo_config & 0xFFFE
	(ok,) = qusb.WriteSetting(QuickUsb.Setting.FifoConfig, t)
	(ok, fifo_config) = qusb.ReadSetting(QuickUsb.Setting.FifoConfig)
	print '\tFifo configuration:', hex(fifo_config)
	
	# Set loop back mode
	print 'Set loop back mode'
	print '\tCommand should read 0.'
	data = QuickUsb.CreateByteBuffer(1)
	address = 0
	data[0] = 0x00
	length = 1
	(ok,) = qusb.WriteCommand(address, data, length)
	(ok, r) = qusb.ReadCommand(address, data, length)
	print '\tCommand:', data[:]
	
	# Set FPGA type to Altera Passive Serial
	print 'Set FPGA type to Altera Passive Serial'
	print '\tFpga type should at least have bit 0 set low.'
	mask = 0xFE
	(ok, fpga_type) = qusb.ReadSetting(QuickUsb.Setting.FpgaType)
	(ok,) = qusb.WriteSetting(QuickUsb.Setting.FpgaType, fpga_type & mask)
	(ok, fpga_type) = qusb.ReadSetting(QuickUsb.Setting.FpgaType)
	print '\tFpga type:', hex(fpga_type)

	# Start FPGA configuration
	time.sleep(0.2)
	print 'Start FPGA Configuration'
	(ok,) = qusb.StartFpgaConfiguration()
	print '\tResult:', ok

	# Read FPGA file and write to FPGA
	print 'Write FPGA Data'
	fpgafile = open("QUSBEVB_REVA_EP2C20_FIFO.rbf","rb")
	fpgadataraw = fpgafile.read()
	fpgafile.close()

	size = len(fpgadataraw)
	fpgadata = QuickUsb.CreateByteBuffer(size)

	for i in xrange(size):
		fpgadata[i] = ord(fpgadataraw[i])

	(ok,) = qusb.WriteFpgaData(fpgadata, size)
	print '\tResult:', ok

	# Check if FPGA is configured
	print 'Is FPGA Configured'
	print '\tConfigured should read as 1.'
	(ok, configured) = qusb.IsFpgaConfigured()
	print '\tConfigured:', configured
	
if configured == 1:
	# Write data
	print 'Writedata'
	print '\tSize:', size
	wdata = QuickUsb.CreateByteBuffer(size)
	
	for i in xrange(size):
		wdata[i] = i%256
	
	(ok,) = qusb.WriteData(wdata, size);
	print '\tResult:', ok
	
	# Read data
	print 'Read data'
	print '\tReadData may fail on a Mac if the size is not a multiple of maxPacketSize.'
	rdata = QuickUsb.CreateByteBuffer(size)
	(ok, bytes) = qusb.ReadData(rdata, size)
	print rdata[:]
	for i in xrange(size):
		if wdata[i] != rdata[i]:
			print '\tRead and write data are not the same at', i
			break
	
	"""print '\tWriteDataAsync'
	#(ok,) = qusb.WriteDataAsync(wdata, size)
	#print '\t\tReturn:', ok
	
	print '\tReadDataAsync'
	#(ok, bytes) = qusb.ReadDataAsync(rdata, size)
	#for i in xrange(size):
	#	if wdata[i] != rdata[i]:
	#		print 'Read and write async data are not the same at ', i
	#		break"""


qusb.Close()
