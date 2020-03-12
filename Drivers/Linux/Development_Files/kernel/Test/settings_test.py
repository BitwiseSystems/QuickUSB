"""============================================================================

Title        : settings_test.py
Description  : QuickUSB settings test
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
#		3. Run script with "python settings_test.py"


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


# QuickUsb Settings
print 'QuickUsb Settings'

# Read fifo config and data address
print '\nReadSetting'
(ok, fifo_config) = qusb.ReadSetting(QuickUsb.Setting.FifoConfig)
print '\tFifo configuration:', hex(fifo_config)
(ok, data_address) = qusb.ReadSetting(QuickUsb.Setting.DataAddress)
print '\tData address:', hex(data_address)

# Write fifoconfig and wordwide, and read their values
print 'WriteSetting'
(ok,) = qusb.WriteSetting(QuickUsb.Setting.FifoConfig, fifo_config+1)
(ok, r) = qusb.ReadSetting(QuickUsb.Setting.FifoConfig)
print '\tFifo configuration + 1:', hex(r)
(ok,) = qusb.WriteSetting(QuickUsb.Setting.WordWide, 0)
(ok, word_wide) = qusb.ReadSetting(QuickUsb.Setting.WordWide)
print '\tWord wide:', hex(word_wide)


# Read fifoconfig default setting
print '\nReadDefault'
(ok, default_fifoconfig) = qusb.ReadDefault(QuickUsb.Setting.FifoConfig)
print '\tDefault fifo configuration:', hex(default_fifoconfig)

# Write fifoconfig default value and read its value
print 'WriteDefault'
(ok,) = qusb.WriteDefault(QuickUsb.Setting.FifoConfig, default_fifoconfig+1)
(ok, r) = qusb.ReadDefault(QuickUsb.Setting.FifoConfig)
print '\tDefault fifo configuration + 1:', hex(r)
(ok,) = qusb.WriteDefault(QuickUsb.Setting.FifoConfig, default_fifoconfig)
(ok, default_fifoconfig) = qusb.ReadDefault(QuickUsb.Setting.FifoConfig)
print '\tDefault fifo configuration:', hex(default_fifoconfig)

print '\nCycle power on module, and run test again. Fifo configuration should be the same as the default fifo configuration.'


qusb.Close()
