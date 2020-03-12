import sys
import time
from QuickUsb import *

LEN = int(sys.argv[1])

(ok, qusbList) = QuickUsb.FindModules()

if len(qusbList) == 0:
    print "Cannot find any modules"
	
qusb = QuickUsb(qusbList[0])
print qusb.Name, qusb.Serial

#qusb.SetTimeout(10000)

arr = CreateByteBuffer(LEN)
stime = time.time()
(ok, bytes) = qusb.ReadData(arr, LEN)
etime = time.time()

print (ok, bytes)
if not ok:
    print "Read failed: ", qusb.LastError()
else:
    print "Data Rate: ", (bytes / (1024.0 * 1024.0)) / (etime - stime), "MB/s"

    if len(sys.argv) > 2 and sys.argv[2] == "show":
        print arr[:]
