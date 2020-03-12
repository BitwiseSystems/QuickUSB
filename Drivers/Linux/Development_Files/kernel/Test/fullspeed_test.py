"""============================================================================

Title        : fullspeed_test.py
Description  : QuickUSB Full Speed test
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
#		3. Run script with "python fullspeed_test.py <value>"
#           a. Full Speed: value == 1
#           b. High Speed: value == 0


import QuickUsb
import sys
import time


# Check for the correct usage
if (len(sys.argv) != 2):
    print 'Usage: python fullspeed_test.py <value>'
    print '\tvalue(0): High Speed'
    print '\tvalue(1): Full Speed'
    sys.exit() 


# Find modules
(ok, modules) = QuickUsb.QuickUsb.FindModules()
if (len(modules) == 0):
	print 'No QuickUSB modules'
	sys.exit()

# Open QuickUSB module
qusb = QuickUsb.QuickUsb(modules[0])
qusb.Open()


# QuickUsb Full Speed
print 'QuickUsb Full Speed'

# Read current CPU configuration
(ok, cpu_config) = qusb.ReadSetting(QuickUsb.Setting.CpuConfig)
print '\tCPU configuration:', hex(cpu_config)

# Create mask for CPU configuration
mask = 1<<15
new_cpu_config = 0

# Update CPU configuration
arg = int(sys.argv[1])
if (arg == 1):
	print "\tSetting full speed..."
	new_cpu_config = cpu_config & ~mask
else:
	print "\tSetting high speed..."
	new_cpu_config = cpu_config | mask
print "\tNew CPU config:", hex(new_cpu_config)

# Write new CPU configuration
(ok,) = qusb.WriteSetting(QuickUsb.Setting.CpuConfig, new_cpu_config);


qusb.Close()

