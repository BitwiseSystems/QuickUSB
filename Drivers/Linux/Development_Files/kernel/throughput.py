import sys
import time
from QuickUsb import *

LEN = 256 * 1024
LOOPS = 50

(ok, qusbList) = QuickUsb.FindModules()

qusb = QuickUsb(qusbList[0])
print qusb.Name, qusb.Serial

arr = CreateByteBuffer(LEN)

totalBytes = 0
stime = time.time()
for k in xrange(LOOPS):
	(ok, bytes) = qusb.ReadData(arr, LEN)
	if not ok or bytes != LEN:
		print "ERROR:", qusb.GetLastError(), ", Bytes read:", bytes
		break
	totalBytes += bytes

etime = time.time()
print "Read data rate: ", (totalBytes / (1024 ** 2)) / (etime - stime)

totalBytes = 0
stime = time.time()
for k in xrange(LOOPS):
	(ok,) = qusb.WriteData(arr, LEN)
	if not ok:
		print "ERROR:", qusb.GetLastError()
		break
	totalBytes += LEN

etime = time.time()
print "Write data rate: ", (totalBytes / (1024 ** 2)) / (etime - stime)

