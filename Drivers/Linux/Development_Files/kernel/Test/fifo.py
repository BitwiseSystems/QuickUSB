import QuickUsb
import sys


# Find modules
(ok, modules) = QuickUsb.QuickUsb.FindModules()
if (len == 0):
	print 'No QuickUSB modules'
	sys.exit()

# Open QuickUSB module
qusb = QuickUsb.QuickUsb(modules[0])
qusb.Open()

# Read data from file
infile = open('data.bin', 'rb')
data = infile.read()
size = len(data)
print 'Write data'
wdata = QuickUsb.CreateByteBuffer(size)
i = 0
for x in data:
	wdata[i] = ord(x)
	i += 1
print wdata[:]
print 'Size:', size
(ok,) = qusb.WriteData(wdata, size);

# Read data
rdata = QuickUsb.CreateByteBuffer(size)
(ok, bytes) = qusb.ReadData(rdata, size)
print 'Read data'
print rdata[:]
print 'Bytes read:', bytes

for i in xrange(size):
	if wdata[i] != rdata[i]:
		print 'Read and write data are not the same.'
		break;

qusb.Close()
