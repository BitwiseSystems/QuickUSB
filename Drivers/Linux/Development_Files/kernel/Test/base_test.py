"""============================================================================

Title        : base_test.py
Description  : QuickUSB base test
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
#		3. Run script with "python base_test.py"


import QuickUsb
import sys


QuickUsb.QuickUsb.SetThrowExceptions(False)


# Find modules
(ok, modules) = QuickUsb.QuickUsb.FindModules()
if (len(modules) == 0):
	print 'No QuickUSB modules'
	sys.exit()

# This is to test if the driver can successfully find all modules two times in a row
(ok, modules) = QuickUsb.QuickUsb.FindModules()
if (len(modules) == 0):
	print 'No QuickUSB modules'
	sys.exit()

# Open QuickUSB module
qusb = QuickUsb.QuickUsb(modules[0])
qusb.Open()


# QuickUsb Base API
print 'QuickUsb Base API'

# This is to test if the Mac can successfully find all modules after an open
# Note: QuickUsbOpen gets and stores a handle to the device. QuickUsbFindModules also
#       gets a handle to the device, but it will close it after it is done. This can
#       cause issues if QuickUsbFindModules closes or invalidates the handle from
#       QuickUsbOpen.
print 'FindModules after Open'
(ok, modules) = QuickUsb.QuickUsb.FindModules()
if (len(modules) == 0):
	print 'No QuickUSB modules'
	sys.exit()
print '\tResult:', ok

print '\nGetStringDescriptor'
(ok, make) = qusb.GetStringDescriptor(1)
print '\tMake:', make
(ok, model) = qusb.GetStringDescriptor(2)
print '\tModel:', model
(ok, serial) = qusb.GetStringDescriptor(3)
print '\tSerial:', serial

print 'GetDriverVersion - Working'
(ok, driver_major, driver_minor, driver_build) = qusb.GetDriverVersion()
print '\tDriver version:', driver_major, driver_minor, driver_build

print 'GetDllVersion - Working'
(ok, dll_major, dll_minor, dll_build) = qusb.GetDllVersion()
print '\tDll version:', dll_major, dll_minor, dll_build

print 'GetFirmwareVersion - Working'
(ok, firmware_major, firmware_minor, firmware_build) = qusb.GetFirmwareVersion()
print '\tFirmware version:', firmware_major, firmware_minor, firmware_build

qusb.Close()


print 'Two sequential open and close pairs'
print '\tThis test should not cause any errors (0).'
qusb.Open()
qusb.Close()
qusb.Open()
qusb.Close()
(error) = qusb.GetLastError()
print '\tError:',error


print 'Two sequential opens and two sequential closes'
print '\tThis test should not cause any errors (0).'
qusb.Open()
qusb.Open()
qusb.Close()
qusb.Close()
(error) = qusb.GetLastError()
print '\tError:',error


print 'Two sequential opens using QuickUsbOpenEx'
print '\tThis test should cause a QUICKUSB_ERROR_ALREADY_OPENED (13).'
qusb.OpenEx(0x0001)
qusb.OpenEx(0x0001)
(error) = qusb.GetLastError()
print '\tError:',error
qusb.Close()

print 'Two sequential closes'
print '\tThis test should cause a QUICKUSB_ERROR_CANNOT_CLOSE_MODULE (14).'
qusb.Open()
qusb.Close()
qusb.Close()
(error) = qusb.GetLastError()
print '\tError:',error


