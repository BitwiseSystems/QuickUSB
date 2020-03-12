"""============================================================================

Title        : fpga_test.py
Description  : QuickUSB FPGA test
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
#		3. Run script with "python fpga_test.py"


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


# FPGA Configuration
print 'FPGA Configuration'

print '\nSet wordwide, turn on FPGA power, set to Altera, and configure FPGA'
print '\tWordword should read 0x1.'
print '\tPort setting should read 0x80-- where -- could vary, but at least bit 7 should be high.'
print '\tFPGA type should read 0x0.'
print '\n\tStartFPGAConfiguration'
(ok,) = qusb.WriteSetting(QuickUsb.Setting.WordWide, 1)
(ok, word_wide) = qusb.ReadSetting(QuickUsb.Setting.WordWide)
print '\t\tWord wide:', hex(word_wide)
(ok,) = qusb.WritePortDir(0, 0x80) # Set port A bit 7 to output
portval = QuickUsb.CreateByteBuffer(1)
portval[0] = 0x80
(ok,) = qusb.WritePort(0, portval, 1) # Set port A bit 7 to high to turn on FPGA power
(ok, porta) = qusb.ReadSetting(QuickUsb.Setting.PortA)
print '\t\tPort A setting:', hex(porta)
mask = 0xFE
(ok, fpga_type) = qusb.ReadSetting(QuickUsb.Setting.FpgaType)
(ok,) = qusb.WriteSetting(QuickUsb.Setting.FpgaType, fpga_type & mask) # Set FPGA type to Altera Passive Serial
(ok, fpga_type) = qusb.ReadSetting(QuickUsb.Setting.FpgaType)
print '\t\tFpga type:', hex(fpga_type)

# This delay is needed before starting the configuration
time.sleep(0.2)

(ok,) = qusb.StartFpgaConfiguration()

print '\nRead FPGA file and write to FPGA'
print '\tWriteFPGAData'
fpgafile = open('QUSBEVB_REVA_EP2C20_FIFO.rbf','rb')
fpgadataraw = fpgafile.read()
fpgafile.close()

size = len(fpgadataraw)
fpgadata = QuickUsb.CreateByteBuffer(size)

for i in xrange(size):
	fpgadata[i] = ord(fpgadataraw[i])

(ok,) = qusb.WriteFpgaData(fpgadata, size)

print '\tIsFPGAConfigured: Configured should read 1 if the FPGA file is valid.'
(ok, configured) = qusb.IsFpgaConfigured()
print '\t\tConfigured:', configured


qusb.Close()

