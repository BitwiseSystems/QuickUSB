"""
============================================================================
Title        : Console.py
Description  : QuickUSB Console App Sample
Notes        : None
History      :

Copyright  2012 Bitwise Systems.  All rights reserved.
This software contains confidential information and trade secrets of
Bitwise Systems and is protected by United States and international
copyright laws.  Use, disclosure, or reproduction is prohibited without
the prior express written permission of Bitwise Systems, except as agreed
in the QuickUSB Plug-In Module license agreement.

Use, duplication or disclosure by the U.S. Government is subject to
restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
(1998), and FAR 12.212, as applicable.

Bitwise Systems
5276 Hollister Ave Suite 304
Santa Barbara, CA  93111
Voice: (805) 683-6469
Fax  : (805) 683-4833
Web  : www.bitwisesys.com
email: support@bitwisesys.com

============================================================================
"""
from QuickUsb import *
import sys

print('QuickUSB Console I2C Demo')

# Find the QuickUSB modules in the system
(ok, modules) = QuickUsb.FindModules()

# Check for no modules and bail if we don't find any
if (len == 0):
    print("Couldn't find any modules\n")
    sys.exit()

# Just use the first device in the list because it's easy
devName = modules[0]

# Print out the name of each module found
for x in modules:
    print("Found", x)

# Open the first device
qusb = QuickUsb(devName)
qusb.Open()

# Write the starting address (0x0000)
write_len = 2
i2c_write_data = CreateByteBuffer(write_len)
i2c_write_data[0] = 0
i2c_write_data[1] = 0
(result,) = qusb.WriteI2C(0x51, i2c_write_data, write_len)
if (result == 0):
    print("Cannot write data to %s" % (devName))
    qusb.Close()
    sys.exit()
print("Set starting I2C memory read address to 0x0000")

# Read I2C bytes
read_len = 8
i2c_read_data = CreateByteBuffer(read_len)
(result,num_read) = qusb.ReadI2C(0x51, i2c_read_data, read_len)
if (result == 0):
    print("Cannot read data from %s" % (devName))
    qusb.Close()
    sys.exit()
print("Read {0} bytes of I2C data starting at 0x0000".format(read_len))

for i in range(read_len):
    print("addr: {0}, data: {1:02X}".format(i, i2c_read_data[i]))

# Close the port when you're done with it
qusb.Close()

print("Done")
